const {fast_type, normal_type} = require('./utils/jsutil')
const tgInst = require('./core/topograph')
// const ccalc = require('./ext/ccalc')

// console.log(ccalc(1,2,3,4))

function pubsubque(){
    this.que = []
    this.subor = []
    this.pub=(msg)=>{
        this.que.push(msg)
        process.nextTick(()=>{
            if(this.que.length <= 0)
                return
            let fm = this.que.shift()
            this.subor.forEach(element => {
                sendrecvque(element, msg)
            });
        })
    }

    this.sub=(id)=>{
        this.subor.push(id)
    }
}

function pushpullque(){

}

function sendrecvque(id, msg){
}

tgInst.bindTo('/root/test')
// pub/sub
// push/pull

let psque = new pubsubque()
let ppque = new pushpullque()

tgInst.setNode('/root/test/pubsubque', null, {obj:psque} )

// let sid = Symbol('test1')
// console.log(sid)

// let str = new String('dddd')
// console.log(str)

// console.log(`${fast_type('')}, ${normal_type('')}`)
// console.log(`${fast_type([])}, ${normal_type([])}`)
// console.log(`${fast_type(1)}, ${normal_type(1)}`)
// console.log(`${fast_type('222')}, ${normal_type('222')}`)
// console.log(`${fast_type(new String('222'))}, ${normal_type(new String('222'))}`)
// console.log(`${fast_type({})}, ${normal_type({})}`)
// console.log(`${fast_type()}, ${normal_type()}`)
// console.log(`${fast_type(true)}, ${normal_type(false)}`)
// console.log(`${fast_type([1])}, ${normal_type([1])}`)
// console.log(`${fast_type(null)}, ${normal_type(null)}`)