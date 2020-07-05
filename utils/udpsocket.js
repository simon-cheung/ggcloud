
var dgram = require('dgram')
var ByteBuffer = require('./ByteBuffer')
var idu = require('../lframe/idutil')
var logutil = require('./logutil')
const {log, loginfo, logerr, logdebug} = logutil
var CommandType = require('./netCode')

function buildUdpSocket(serverPort, bindip, options){
    let serverSock = dgram.createSocket('udp4')
    serverSock.left = serverSock.on
    
    serverSock.left('close', ()=>{

    })

    serverSock.left('listening', ()=>{

    })

    serverSock.bind(serverPort, bindip)

    return serverSock
}

module.exports = buildUdpSocket
