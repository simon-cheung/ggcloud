const uuid = require('uuid')
const {DFsm, BTree} = require('./dfsm')

const serviceTop = {
    chat_root :'/root/chat',
    channel:(x)=>{return `/root/chat/channel/${x}`},
}

const stateX = [
    'idle', 'wait_login', 'login', 'wait_join', 'in_channel', 'logout'
]

const actionX = [
    'login', 'login_err', 'login_suc', 'join_channel', 'join_err', 'join_suc', 'leave_channel', 'send_txt', 'logout'
]

const transitArrow = [
    {equal:'idle', by:'login', assign:'wait_login'},
    {equal:'wait_login', by:'login_err', assign:'idle'},
    {equal:'wait_login', by:'login_suc', assign:'login'},
    {equal:'login', by:'join_channel', merge:'wait_join'},
    {has:'wait_join', by:'join_err', assign:'', remove:'wait_join'},
    {has:'wait_join', by:'join_suc', assign:'in_channel', remove:''},
    {has:'in_channel', by:'leave_channel', assign:'', remove:'in_channel'},
    {has:'in_channel', by:'send_txt', assign:'', remove:''},
    {equal:'login', by:'logout', remove:'idle'},
]

const chatBTree = {
    blackboard:{},
    btree:{

    }
}

function DAgent(id, cli){
    this.id = id
    this.channel = null
    this.fsm = new DFsm(stateX, actionX, 'idle')

    // declare msg entry
    cli.add_msg_entry(this)

    this.act_idle=(blackboard)=>{

        return BTree.BT_Running
    }

    this.build=()=>{

    }
    
    this.update=()=>{        
    }
}

function DCli(msgproxy){
    this.idbase = 1000
    this.id = uuid.v1()
    this.agents = {}
    this.random = (cnt)=>{
        this.idbase++
        let agent = new DAgent(this.idbase, msgproxy)
        this.agents[agent.id] = agent
    }

    this.add_msg_entry = (agent)=>{
        msgproxy.add_msg_entry(`/${this.id}/${agent.id}`, agent.on.bind(agent))
    }

    this.rem_msg_entry = (agent)=>{
        msgproxy.rem_msg_entry(`/${this.id}/${agent.id})`)
    }

    this.send_msg = (agent, to, msg)=>{

    }

    this.tick = ()=>{

    }
}

module.exports = DCli