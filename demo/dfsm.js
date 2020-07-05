const R=require('ramda')
const idutil = require('../utils/idutil')
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
    this.nodes = {}
    this.stack = []
}

BTree.BT_Success = 0
BTree.BT_Fail =-1
BTree.BT_Running = 1

BTree.prototype._buildNode = function(nodePrefix, nodeSymbol, parentNode){
    let node
    let parent
    let id = nodeSymbol
    if(id){
        id = Symbol(`${nodePrefix}_${idutil.genId()}`)   
    }

    parent = parentNode || this.root
    node = this.nodes[nodeSymbol]

    if(!node){
        node = {parent, id}
        this.nodes[id] = node
        parent[id] = node
    }

    if(node.parent !== parentNode){
        delete node.parent[id]
        node.parent = parent
        node.parent[id] = node
    }

    return node
}

BTree.prototype.selectorNode = function(nodeSymbol, parentNode){
    let node = this._buildNode('selector', nodeSymbol, parentNode)
    node.childs = []
    node.run = BTree.selector.bind(this)
    return node
}

BTree.prototype.sequenceNode = function(nodeSymbol, parentNode){
    let node = this._buildNode('sequence', nodeSymbol, parentNode)
    node.childs = []
    return node
}

BTree.prototype.parallelNode = function(nodeSymbol, parentNode){
    let node = this._buildNode('parallel', nodeSymbol, parentNode)
    node.childs = []
    return node
}

BTree.prototype.actionNode = function(nodeSymbol, parentNode){
    let node = this._buildNode('action', nodeSymbol, parentNode)
    node.run = null
    return node
}

BTree.prototype.conditionNode = function(nodeSymbol, parentNode){
    let node = this._buildNode('condition', nodeSymbol, parentNode)
    node.run = null
    return node
}

// flow

BTree.prototype.runNode = function(node){
    this.stack.push(node)
    let res = node.run(this.blackboard)
    if(res != BTree.BT_Running)
        return res
    this.stack.pop()
    return res
}

BTree.prototype.selector = function(node){
    for(let leaf of node.childs){
        let res = leaf.run(blackboard)

        if(res == BTree.BT_Success)
            return BTree.BT_Success
        if(res == BTree.BT_Fail)
            continue
        if( res == BTree.BT_Running){
            blackboard.running.shitf(leaf)
            return BTree.BT_Running
        }
    }
    return BTree.BT_Fail
}

BTree.prototype.sequence = function(node, blackboard){
    for(let leaf of node.childs){
        let res = leaf.run(blackboard)

        if(res == BTree.BT_Success)
            continue
        if(res == BTree.BT_Fail)
            return BTree.BT_Fail
        if( res == BTree.BT_Running){
            blackboard.running.shitf(leaf)
            return BTree.BT_Running
        }
    }
    return BTree.BT_Success
}

BTree.prototype.parallel = function(node, blackboard){
    let state = BTree.BT_Success
    for(let leaf of node.childs){
        let res = leaf.run(blackboard)

        if(res == BTree.BT_Success)
            continue
        if(res == BTree.BT_Fail)
            state = BTree.BT_Fail
        if( res == BTree.BT_Running){
            blackboard.running.shitf(leaf)
            return BTree.BT_Running
        }
    }
    return state
}

module.exports = {DFsm, BTree}