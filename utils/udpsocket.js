
var dgram = require('dgram')
var ByteBuffer = require('./ByteBuffer')
var idu = require('../lframe/idutil')
var logutil = require('./logutil')
const {log, loginfo, logerr, logdebug} = logutil
var CommandType = require('./netCode')

function S_USocket(serverPort, bindip, options){
    this.port = serverPort
    this.bindip = bindip
    this.serverSock = dgram.createSocket('udp4')
}
