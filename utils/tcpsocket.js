
var net = require('net')
var ByteBuffer = require('./ByteBuffer')
var idu = require('../lframe/idutil')
var logutil = require('./logutil')
const {log, loginfo, logerr, logdebug} = logutil

var CommandType = require('./netCode')

function S_WSocket(serverPort, bindip, optionos) {
    var that = this
    this.port = serverPort
    this.bindip = bindip || 'localhost'
    this.serverSock = net.createServer(function(sock){
        // connection
        connectionEstablished(that, sock, CommandType.ACCEPT_RESULT)
    })

    this.serverSock.listen(serverPort, bindip)    
}

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
    this.sock = new net.Socket()
    this.sock.connect(serverPort, serverIp, function(){
        // connection
        connectionEstablished(that, that.sock, CommandType.CONNECT_RESULT)
    })
}

function connectionEstablished(socektHold, ws, code) {
    var sessionId = idu.sid()
    ws.setKeepAlive(true, 1000)
    ws.setNoDelay(true)
    // ws.setTimeout(3000)
    ws.sessionId= sessionId
    ws.serverPort = socektHold.port
    ws.socketType = 'Socket'
    socektHold.hub.socketMap[sessionId]=ws
    
    log("connection established, sessionId:"+sessionId)

    var bindObject = {
        s_wsocket: socektHold,
        socket: ws
    }
    ws.on('data', handleMessage.bind(bindObject))
    ws.on('error', error.bind(bindObject))
    ws.on('close', close.bind(bindObject))
    ws.on('timeout', timeout.bind(bindObject))
    
    ws.sendToPeer = sendToPeer.bind(ws)
    ws.closePeer = closePeer.bind(ws)

    var obj={}
    obj.sessionId=ws.sessionId
    obj.port = socektHold.port
    obj.serverIp = socektHold.serverIp
    socektHold.proc(code,obj)    
}

function timeout(e){
    var s_wsocket=this.s_wsocket
    var socket=this.socket
    socket.destroy();
    close.apply(this, e)
    log("timeout session !!!!!socket.sessionId:"+socket.sessionId)        

}

function error(e){
    // net error ,do it with close()
    log(`client connection error, session : ${this.socket.sessionId}`)
}

function close(error) {
    var s_wsocket=this.s_wsocket
    var socket=this.socket
    delete s_wsocket.hub.socketMap[socket.sessionId]
    // idu.revId(socket.sessionId)
    // if(error){  // closed by net error
        log("closed session !!!!!socket.sessionId:"+socket.sessionId)        
        var obj={}
        obj.sessionId=socket.sessionId
        this.s_wsocket.proc(CommandType.USER_CONN_CLOSE,obj)
    // }
}

function handleMessage(message) {
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

        if(len <= 0 || len > 10*1024*1024){
            console.log(`invalid package length`)
            this.socket.closePeer()
            this.socket.emit('error')
            return
        }

        var obj={}
        obj.sessionId=this.socket.sessionId
        // obj.msgNumber = msgNumber
        obj.byteBuf = byteBuf.readUint8Array(len)

        this.s_wsocket.proc(CommandType.RECV_MSG_FROM_USER,obj)
    }
    // 清空已经使用的数据
    byteBuf.splitUsedBuffer()
    // this.s_wsocket.hub.sendToProcess(CommandType.RECV_MSG_FROM_USER,obj)
}

function sendToPeer(buffer) {
    //log("发送消息:"+msgNumber)
    this.write(buffer)
}

function closePeer(buffer){
    this.end(buffer)
}
module.exports = {S_Socket:S_WSocket, C_Socket:C_WSocket}