
var M={}

M.idbase = 1000
M.sidbase = 1000
M.idpool=[]

M.genId = ()=>{
    if(M.idpool.length == 0){
        for(var i=0; i<1000; i++){
            M.idpool.push(i + M.idbase)
        }
        M.idbase += 100000
    }
    return M.idpool.pop()
}

M.revId = (id)=>{
    M.idpool.unshift(id)
}

M.sid = ()=>{
    M.sidbase++
    if(M.sidbase > 999999999)
        M.sidbase = 1000
    return Symbol(`${M.sidbase}`)
}
module.exports = M