
function DFsm(arrState, arrAction, defaultState){
    this.state = defaultState || arrState[0]

    this.link = (s, a)=>{

    }

    this.arrow = (s, s1, a)=>{
        
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

module.exports = BTree