
const _ = require('lodash')
const path = require('path')
const tgInst = require('./topograph')

function TopoMsg(){
    this.from = 'localhost'
    this.to = 'peer'
    this.buffer = null
}

TopoMsg.prototype.fillData = function(buffer){

}

TopoMsg.prototype.step = function(){

}

// pub/sub, req/resp, push/pull, send/recv
TopoMsg.prototype.pub = function(from, to, opt){

}

TopoMsg.prototype.req = function(from, to, opt){

}

TopoMsg.prototype.push = function(from, to, opt){

}

TopoMsg.prototype.send = function(from, to, opt){

}

// right msg util
TopoMsg.localPath = '/'
TopoMsg.localAddr = function(subpath){
    return path.join(this.localPath, subpath)
}
