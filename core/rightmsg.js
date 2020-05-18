
const _ = require('lodash')
const path = require('path')

function RightMsg(){
    this.from = 'localhost'
    this.to = 'peer'
    this.buffer = null
}

RightMsg.prototype.fillData = function(buffer){

}

RightMsg.prototype.step = function(){

}

// pub/sub, req/resp, push/pull, send/recv
RightMsg.prototype.pub = function(from, to, opt){

}

RightMsg.prototype.req = function(from, to, opt){

}

RightMsg.prototype.push = function(from, to, opt){

}

RightMsg.prototype.send = function(from, to, opt){

}

// right msg util
RightMsg.localPath = '/'
RightMsg.localAddr = function(subpath){
    return path.join(this.localPath, subpath)
}
