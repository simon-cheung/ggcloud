const R=require('ramda')
// 1. from and to,will set toState, rem fromState
// 2. has and insert, will insert insertState, rem all remState
// 3. when (1), will clean all other state by call [has/insert]

// 并集
const union = (a,b)=>new Set([...a, ...b]);
const intersect = (a, b)=>new Set([...a].filter(x => b.has(x)));
const difference = (a, b)=>new Set([...a].filter(x => !b.has(x)));

const only=(a, set)=>R.equals([a], set)

function DFsm(defaultState){
    this.state = new Set([defaultState])
    this.rules = null
    this.static_by = {}
    
    this.setRules = (arrows)=>{
        this.rules = arrows
        // split by action
        for(let a of arrows){
            this.addRule(a)
        }
    }

    this.addRule = (arrow)=>{
        if(arrow.by){
            let byarr = this.static_by[a.by]
            if(!byarr){
                byarr = []
                this.static_by[a.by] = byarr
            }
            byarr.push(a)
        }
    }

    this.can = ()=>{
        for(let by of this.static_by){

        }
    }

    this.transitBy = (action)=>{
        let byarr = this.by[action]
        for(let arrow of byarr){
            
        }
    }
}

DFsm.prototype.justAny = (...state)=>{
    for(let s of this.state){
        if(!state.has(s))
            return false
    }
    return true
}

DFsm.prototype.just = (...state)=>{
    if (this.state.size !== state.size) return false;
    for(let s of this.state){
        if(!state.has(s))
            return false
    }
    return true
}

DFsm.prototype.hasAny = (...state)=>{
    for(let s of state){
        if(this.state.has(s))
            return true
    }
    return false
}

DFsm.prototype.hasAll = (...state)=>{
    for(let s of state){
        if(!this.state.has(s))
            return false
    }
    return true
}

DFsm.prototype.remove = (...state)=>{
    for(let s of state){
        this.state.delete(s)
    }
}

DFsm.prototype.set = function (...state){
    this.state.clear()
    for(let s of state){
        this.state.add(s)
    }        
}

DFsm.prototype.insert = function (...state){
    for(let s of state){
        this.state.add(s)
    }         
}

function BTree(blackboard){
    this.board = blackboard
    this.root = {}
}

BTree.BT_Success = 0
BTree.BT_Fail =-1
BTree.BT_Running = 1

// build tree
BTree.prototype.build_action = function(aid, act_cb){

}

BTree.prototype.build_condition = function(aid, act_cb){

}

BTree.prototype.build_composite = function(adi){

}

BTree.prototype.transit = function(state){

}

BTree.prototype.emit = function(evt, ...params){

}

module.exports = {DFsm, BTree}