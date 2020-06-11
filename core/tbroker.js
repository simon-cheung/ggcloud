const {smgrInst} = require('../utils/sessionMgr')
const netcode = require('../utils/netCode')
const byteBuffer = require('../utils/byteBuffer')
const idutil = require('../utils/idutil')

let defaultOpt = {
    host:'localhost',
    port:3101
}

function TBroker(opt){
    let lopt = {}
    Object.assign(lopt, defaultOpt)
    this.opt = lopt

    smgrInst.startServer(lopt.port, 'Tcp', this.procNetEvent.bind(this), lopt.host)
}

TBroker.prototype.procNetEvent = function(ncode, obj){
    switch(ncode){
        case netcode.ACCEPT_RESULT:{

        }break
        case netcode.RECV_MSG_FROM_USER:{

        }break;
        case netcode.USER_CONN_CLOSE:{

        }break;
    }
}