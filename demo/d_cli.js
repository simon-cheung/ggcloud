const uuid = require('uuid')
const serviceTop = {
    chat_root :'/root/chat',
    channel:(x)=>{return `/root/chat/channel/${x}`},
}

const stateX = [
    'idle', 'wait_login', 'login', 'wait_join', 'in_channel', 'logout'
]
const actionX = [
    'login', 'login_err', 'login_suc', 'join_channel', 'leave_channel', 'send_txt', 'logout'
]

function DAgent(id, cli){
    this.id = id
    this.channel = null
    this.fsm = {}

    // declare msg entry
    cli.add_msg_entry(this)

    this.login = function(){
        cli.send_msg(this, serviceTop.chat_root, {code:'login', id})
    }

    this.join_channel = function(name){
        cli.send_msg(this, serviceTop.chat_root, {code:'join', channel:name})
    }

    this.leave_channel = function(){
        cli.send_msg(this, serviceTop.chat_root, {code:'leave'})
    }

    this.send_txt = function(){
        cli.send_msg(this, serviceTop.channel(this.channel), {code:'txt', txt:'hello world'})
    }

    this.logout = function(){
        cli.send_msg(this, serviceTop.chat_root, {code:'logout', id})
    }

    this.on = function(body){
        let mbody = JSON.parse(body)
        console.log(`id ${id}: ${body.code}`)

        switch(body.code){
            case 'join':{
                this.channel = mbody.channel
            }
        }
    }

    this.canAction = (action)=>{
        if(action == 'login'){
            return this.fsm.has(logout)
        }
        if(action == 'join_channel'){
            return this.fsm.has('in_channel')
        }
        if(action == 'l')
    }

    this.transit = (state, params)=>{
        this.fsm[state] = params
    }

    this.autoBehavior = ()=>{
        let ncmd = Math.floor(Math.random() * 10000) % 5
        let action = actionX[ncmd]
        if(this.canAction(actionX)){

        }
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