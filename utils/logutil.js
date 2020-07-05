

let logingore = ()=>{}
let M={}
M.log = logall
M.loginfo = logall
M.logerr = logall
M.logdebug = logdebug
M.bFork = false
M.bDebug = false

function logall(l){
    var ou = ''
    if(M.bFork){
        ou = `P ${process.pid}->> ${l}`
    }else{
        ou = l
    }
    console.log(ou)
}

function logdebug(l){
    if(M.bDebug)
        logall(l)
}

M.setup=(bfork, bdebug)=>{
    M.bFork = bfork
    if(bdebug)
        M.bDebug = bdebug
}


module.exports=M