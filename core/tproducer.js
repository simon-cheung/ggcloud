const netcode = require('../utils/netCode')
const byteBuffer = require('../utils/byteBuffer')
const idutil = require('../utils/idutil')

let defaultOpt = {
    host:'localhost',
    port:3101
}

function TProducer(opt){
    let lopt = {}
    Object.assign(lopt, defaultOpt)
    this.opt = lopt
}
