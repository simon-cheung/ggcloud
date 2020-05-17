
const httpsServ = require('https')
const httpServ = require('http')
const WebSocket = require('ws')
const io = require('socket.io')
const idu = require('../lframe/idutil')
const ByteBuffer = require('./ByteBuffer')

const logutil = require('./logutil')
const {log, loginfo, logerr, logdebug} = logutil

const CommandType = require('./netEventCode')

const processRequest = (req, res) => {
    res.writeHead(200);
    res.end('Hello, Player!\n');
}

function S_WSocket(serverPort, bindip, options) {
    var that = this
    let http = null
    // if(options && options.ssl_key && options.ssl_crt){
    //     let http = httpsServ.createServer({key:options.ssl_key, cert:options.ssl_crt}, processRequest).listen(serverPort, bindip)
    //     this.socketServer = new WebSocket.Server({server:http});
    // }else{
    //     let http = httpServ.createServer(processRequest).listen(serverPort, bindip)
    //     this.socketServer = new WebSocket.Server({server:http});
    // }
    // this.socketServer = new WebSocket.Server({host:bindip, port:serverPort});
    if(options && options.ssl_key && options.ssl_crt){
        let http = httpsServ.createServer({key:options.ssl_key, cert:options.ssl_crt}, processRequest).listen(serverPort, bindip)
        this.socketServer = io(http)
    }else{
        let http = httpServ.createServer(processRequest).listen(serverPort, bindip)
        this.socketServer = io(http,{
            serveClient:false,
            transports:['websocket'],
            pingInterval:10000,
            pingTimeout:5000,
            cookie:false
            })
    }    
    this.socketServer.on('connection', function(sock, req){
        // connection
        // sock.remoteAddress = req.client.remoteAddress
        // sock.remoteFamily = req.client.remoteFamily
        sock.remoteAddress = sock.request.connection.remoteAddress
        sock.remoteFamily = sock.request.connection.remoteFamily
        
        connectionEstablished(that, sock, CommandType.ACCEPT_RESULT)
    })
    // Properly handle errors because some people are too lazy to read the readme
    this.socketServer.on('error', function err(e) {
        switch (e.code) {
            case "EADDRINUSE":
                console.log("[Error] Server could not bind to port! Please close out of Skype or change 'serverPort' in gameserver.ini to a different number.");
                break;
            case "EACCES":
                console.log("[Error] Please make sure you are running Ogar with root privileges.")
                break;
            default:
                console.log("[Error] Unhandled error code: " + e.code)
                break;
        }
    });
};

S_WSocket.prototype.stop=function(){
    this.serverSock.close()
    var smap = Object.keys(this.hub.socketMap)
    for(var k of Object.keys(smap)){
        var s = smap[k]
        if(s.serverPort == this.port){
            // idu.revId(k)
            s.close()
            delete smap[k]
        }
    }
    this.hub = null
    this.serverSock = null
}

function C_WSocket(serverPort, serverIp){
    var that = this
    this.port = serverPort
    this.serverIp = serverIp
    var url =`ws://${serverIp}:${serverPort}/ws`
    this.sock = new WebSocket(url, {
        perMessageDeflate: false
    });
    this.sock.on('open', function(){
        connectionEstablished(that, that.sock, CommandType.CONNECT_RESULT)
    })
}

function connectionEstablished(socektHold, ws, code) {
    var sessionId = idu.sid()
    // ws.setKeepAlive(true, 1000)
    ws.sessionId= sessionId
    ws.serverPort = socektHold.port
    ws.socketType = 'WebSocket'
    socektHold.hub.socketMap[sessionId]=ws
    
    log("connection established, sessionId:"+sessionId)

    var bindObject = {
        s_wsocket: socektHold,
        socket: ws
    }

    ws.on('message', handleMessage.bind(bindObject));
    // ws.on('error', error.bind(bindObject))
    // ws.on('close', close.bind(bindObject))
    ws.on('disconnect', close.bind(bindObject))

    ws.sendToPeer = sendToPeer.bind(ws)
    ws.closePeer = closePeer.bind(ws)

    var obj={}
    obj.sessionId=ws.sessionId
    obj.port = socektHold.port
    obj.serverIp = socektHold.serverIp
    socektHold.proc(code,obj)        
};


function error(e){
    // net error ,do it with close()
    log(`client connection error, session : ${this.socket.sessionId}`)
}

function close(error) {
    var s_wsocket=this.s_wsocket;
    var socket=this.socket;
    delete s_wsocket.hub.socketMap[socket.sessionId]
    // idu.revId(socket.sessionId)
    log("closed session !!!!!socket.sessionId:"+socket.sessionId)        

    var obj={};
    obj.sessionId=socket.sessionId;
    this.s_wsocket.proc(CommandType.USER_CONN_CLOSE,obj)
}

function handleMessage(message) {
    // var byteBuf=new ByteBuffer();
    // byteBuf.initBlank();
    // byteBuf.setMsg(message);
    // var msgNumber=byteBuf.readInt16();    
    // var body=MsgBuild(msgNumber,byteBuf);

    // var obj={
    //     body:body,
    //     msgNumber:msgNumber,
    //     sessionId:this.socket.sessionId
    // }

    // this.s_wsocket.child.sendToProcess(CommandType.RECV_MSG_FROM_USER,obj,true);

    if(!this.socket.buf){
        this.socket.buf=new ByteBuffer()        
        this.socket.buf.initBlank()
    }
    var byteBuf = this.socket.buf
    byteBuf.appendMsg(message)
    while(byteBuf.unused() > 4){
        // if(byteBuf.unused() < 4){   // length        
        //     return
        // }
        var len = byteBuf.readInt()
        if(len > byteBuf.unused()){
            byteBuf.reuse(4)
            break
        }

        // var msgNumber=byteBuf.readInt16()

        var obj={}
        obj.sessionId=this.socket.sessionId
        // obj.msgNumber = msgNumber
        obj.byteBuf = byteBuf.readUint8Array(len)

        this.s_wsocket.proc(CommandType.RECV_MSG_FROM_USER,obj)
    }
    // 清空已经使用的数据
    byteBuf.splitUsedBuffer()
    // this.s_wsocket.hub.sendToProcess(CommandType.RECV_MSG_FROM_USER,obj)
};

function sendToPeer(buffer) {
    //log("发送消息:"+msgNumber)
    this.send(buffer)
}

function closePeer(buffer){
    var that = this
    if(buffer){
        this.send(buffer, function(){
            that.terminate()
        })    
    }else{
        that.terminate()
    }
    // this.end(buffer)
}

module.exports = {S_Socket:S_WSocket, C_Socket:null}
