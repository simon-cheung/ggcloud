const _ = require('lodash')
const {fast_type, normal_type} = require('../utils/jsutil')
const {posix} = require('path')
const TopoPos = require('./topopos')


function topoPath(gpath){
    if( fast_type(gpath) == 'string'){
        gpath = posix.normalize(gpath)
        let sg = gpath.split('/').filter(x=>x)
        let short_path = sg[sg.length - 1]
        let full_path = gpath
        return {full_path, short_path, lodash_key:sg}
    }

    return {full_path:null, short_path:null, lodash_key:null}
}

function TopoGraph(){
    this.bindTo('/root')
}

TopoGraph.prototype.bindTo = function(global_pos){
    const {lodash_key} = topoPath(global_pos)
    this.root = _.set(lodash_key, {__state:{}, __props:{}})    
}

TopoGraph.prototype.setNode=function(pos, state, props){
    const {lodash_key} = topoPath(pos)
    state = state || {}
    props = props || {}
    let old = _.get(lodash_key)
    if(old){
        old.__state = state
        old.__props = props
    }else{
        _.set(lodash_key, {__state:state, __props:props})
    }
}

TopoGraph.prototype.mergeNode=function(pos, state, props){
    const {lodash_key} = topoPath(pos)
    state = state || {}
    props = props || {}    
    let old = _.get(lodash_key)
    if(old){
        Object.assign(old.__state, state)
        Object.assign(old.__props, props)
    }else{
        _.set(lodash_key, {__state:state, __props:props})
    }
}

TopoGraph.prototype.getNode=function(pos){
    const {lodash_key} = topoPath(pos)
    let old = _.get(lodash_key)
    return old
}

TopoGraph.prototype.getState=function(pos){
    const {lodash_key} = topoPath(pos)
    let old = _.get(lodash_key)
    if(old){
        return old.__state
    }else{
        return null
    }
}

TopoGraph.prototype.getProps=function(pos){
    const {lodash_key} = topoPath(pos)
    let old = _.get(lodash_key)
    if(old){
        return old.__props
    }else{
        return null
    }
}

TopoGraph.prototype.setState=function(pos, state){
    const {lodash_key} = topoPath(pos)
    let old = _.get(lodash_key)
    state = state || {}
    if(old){
        old.__state = state
    }else{
        _.set(lodash_key, {__state:state, __props:{}})
    }
}

TopoGraph.prototype.setProps=function(pos, props){
    const {lodash_key} = topoPath(pos)
    let old = _.get(lodash_key)
    props = props || {}
    if(old){
        old.__props = props
    }else{
        _.set(lodash_key, {__state:{}, __props:props})
    }
}

TopoGraph.prototype.mergeState=function(pos, state){
    const {lodash_key} = topoPath(pos)
    let old = _.get(lodash_key)
    state = state || {}
        
    if(old){
        Object.assign(old.__state, state)
    }else{
        _.set(lodash_key, {__state:state, __props:{}})
    }
}

TopoGraph.prototype.mergeProps=function(pos, props){
    const {lodash_key} = topoPath(pos)
    let old = _.get(lodash_key)
    props = props || {}    
    if(old){
        Object.assign(old.__props, props)
    }else{
        _.set(lodash_key, {__state:{}, __props:props})
    }
}

let tgInst = new TopoGraph()
module.exports = tgInst

