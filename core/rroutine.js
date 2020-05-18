const {smgrInst} = require('../utils/sessionMgr')
const netcode = require('../utils/netCode')
const byteBuffer = require('../utils/byteBuffer')
const idutil = require('../utils/idutil')

let defaultOpt = {
    host:'localhost',
    port:3101,
    cookie:'ieNfy(u&i292'
}

function RRoutine(opt){
    let lopt = {}
    Object.assign(lopt, defaultOpt)
    this.opt = lopt

    smgrInst.connectServer(lopt.port, 'Tcp', lopt.host, this.procNetEvent.bind(this))
}

RRoutine.prototype.procNetEvent = function(ncode, obj){
    switch(ncode){
        case netcode.CONNECT_RESULT:{

        }break
        case netcode.RECV_MSG_FROM_USER:{

        }break;
        case netcode.USER_CONN_CLOSE:{

        }break;
    }
}