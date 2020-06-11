const _ = require('lodash')
const {fast_type, normal_type} = require('../utils/jsutil')
const {posix} = require('path')


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

function TopoPos(gpath){
    // this.child = {} parent & child is a topoedge
    if(gpath === null || gpath === undefined){
        gpath = '/root'
    }

    if( fast_type(gpath) == 'string'){
        gpath = posix.normalize(gpath)
        let sg = gpath.split('/').filter(x=>x)
        this.short_path = sg[sg.length - 1]
        this.global_path = gpath
    }else if( gpath instanceof TopoPos){
        this.global_path = gpath.global_path
        this.short_path = gpath.short_path
    }else{
        throw 'invalid gpath for topology pos'
    }
}

module.exports = TopoPos