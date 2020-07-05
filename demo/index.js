const {DFsm, BTree} = require('./dfsm')
const idutil = require('../utils/idutil')
const _ = require('lodash')
const async_step = (x)=>{ process.nextTick(x)}
const buildUdpSocket = require('../utils/udpsocket')


// chat robot
let blackboard = {}
let bot = {}

let server = buildUdpSocket(1111)
bot.btree = new BTree({})

let mainseq = bot.btree.sequenceNode()
let con_login = bot.btree.conditionNode(null, mainseq)
con_login.run = (blackboard)=>{ blackboard.con_login == bot.is_login; return BTree.BT_Success}
let do_login = bot.btree.actionNode(null, mainseq)
do_login.run = (blackboard)=>{ 
    // tcp consocket
    if(!bot.login_sock)
        bot.do_login()
    if(!bot.login_peer)
        return BTree.BT_Running
    return BTree.BT_Success
}
