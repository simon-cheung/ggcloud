const Socket = require("./tcpsocket")
const WebSocket = require("./websocket")
// var WebSocket = require("./siosocket")
const ByteBuffer=require('./byteBuffer')
const CommandType = require('./netCode')
const logutil = require('./logutil')
const {log, loginfo, logerr, logdebug} = logutil

function pingT() {
    setTimeout(pingT, 10)
    for (var key in that.socketMap) {
        var socket = that.socketMap[key]
        if (socket != null && socket != undefined) {
            try {
                socket.ping()
            }catch (err) {
                log("ping err:" + err)
            }
        }
    }
}

function SessionMgr() {
    this.socketMap = {}
    this.serverSocketMap={}
    //setTimeout(pingT,10)
}

SessionMgr.prototype.getSessionInfo = function(sid){
    return this.socketMap[sid]
}

SessionMgr.prototype.startServer=function(port, netservice, proc, bindip, options){
    var that = this
    var service=null
    if(netservice == 'WebSocket')
        service = WebSocket.S_Socket
    else
        service = Socket.S_Socket
    var socketServer = new service(port, bindip, options)
    socketServer.hub = this
    socketServer.proc=proc
    this.serverSocketMap[port] = socketServer
}

SessionMgr.prototype.connectServer=function(port, netservice, serverIp, proc){
    var that = this
    var service=null
    if(netservice == 'WebSocket')
        service = WebSocket.C_Socket
    else
        service = Socket.C_Socket
    var socketClient = new service(port, serverIp)
    socketClient.hub = this
    socketClient.proc=proc
    // this.serverSocketMap[port] = socketClient
}

SessionMgr.prototype.send = function (type, obj) {
    var that = this
    var msgNumber = obj.msgNumber || 0
    var body = obj.body
    var sessionId = obj.sessionId

    if (type == CommandType.SEND_MSG_TO_USER)
    {
        that.sendTo(sessionId, msgNumber, body)
    }else if(type == CommandType.SEND_MSG_TO_USER_THEN_CLOSE){
        that.sendToThenClose(sessionId, msgNumber, body)

    }else if (type == CommandType.SEND_MSG_TO_SOME_USER){
        for (var i = 0; i < obj.sessionIds.length; i++) {
            sessionId = obj.sessionIds[i]
            that.sendTo(sessionId, msgNumber, body)
        }
    }else if (type == CommandType.SEND_MSG_TO_SOME_USER_THEN_CLOSE){
        for (var i = 0; i < obj.sessionIds.length; i++) {
            sessionId = obj.sessionIds[i]
            that.sendToThenClose(sessionId, msgNumber, body)
        }
        
    }else if (type == CommandType.SEND_MSG_TO_ALL_USER){
        that.sendAll(msgNumber, body)

    }else if (type == CommandType.SEND_MSG_TO_ALL_USER_THEN_CLOSE){
        that.sendAll(msgNumber, body, true)

    }else if (type == CommandType.CLOSED_SESSION){
        var socket = that.socketMap[sessionId]
        delete that.socketMap[sessionId]
        if (socket != null && socket != undefined) {
            socket.destroy()
        }else {
            log('cant find the sessionId [' + sessionId + '] for closing')
        }
        log('force close session :' + sessionId)

    }
    else if (type == CommandType.CLOSE_ALL_SESSION)//全部关闭
    {

        //for(var sessionId in that.socketMap)
        //{
        //    var socket=that.socketMap[sessionId]
        //    if(socket!=null&&socket!=undefined)
        //    {
        //       socket.close()
        //    }
        //    delete that.socketMap[sessionId]
        //}
        log('close all session')

    }

}

SessionMgr.prototype.closeSession=function(sid){
    this.send(CommandType.CLOSED_SESSION,{ sessionId:sid})
}

SessionMgr.prototype.sendAll = function (msgNumber, body, thenClose) {
    var buffer = buildPackage(msgNumber, body)

    for (var key in this.socketMap) {
        var socket = this.socketMap[key]
        if (socket != null && socket != undefined) {
            if(thenClose){
                socket.closePeer(buffer)
            }else{
                socket.sendToPeer(buffer)
            }
        }
    }
    // this.socketServer.sendToSomePeer(socks, msgNumber, body)
}

SessionMgr.prototype.sendTo = function (sessionId, msgNumber, body) {
    var socket = this.socketMap[sessionId]
    //log("sessionId:"+sessionId+"  "+socket+"  "+Object.keys(this.socketMap).length)
    if (socket != null && socket != undefined) {
        //log("发送消息:"+msgNumber)
        var buffer = buildPackage(msgNumber, body)
        socket.sendToPeer(buffer)
    }
}

SessionMgr.prototype.sendToThenClose = function (sessionId, msgNumber, body) {
    var socket = this.socketMap[sessionId]
    //log("sessionId:"+sessionId+"  "+socket+"  "+Object.keys(this.socketMap).length)
    if (socket != null && socket != undefined) {
        //log("发送消息:"+msgNumber)
        var buffer = buildPackage(msgNumber, body)
        socket.closePeer(buffer)
    }
}

function buildPackage(msgNumber, body){
    var slen = body.length + 4
    var sbuf = new ArrayBuffer(slen)
    var lview = new DataView(sbuf)
    lview.setInt32(0, body.length, true)
    var tu8arr = new Uint8Array(sbuf)
    tu8arr.set(body, 4)

    var buffer = Buffer.from(sbuf)
    return buffer
}

SessionMgr.prototype.stopServer=function(port){
    if(this.serverSocketMap[port]){
        this.serverSocketMap[port].stop()
        delete this.serverSocketMap[port]
    }
}

SessionMgr.smgrInst = new SessionMgr()

module.exports=SessionMgr