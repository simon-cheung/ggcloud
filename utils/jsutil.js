function fast_type(variable) {
    if (variable === null )
        return 'null'
    if(variable === undefined) {
        return 'undefined';
    }
    var cst = variable.constructor;

    switch (cst) {
        case Number:
            return 'number'
        case String:
            return 'string'
        case Boolean:
            return 'boolean'
        case Array:
            return 'array'
        case Object:
            return 'object'
    }
}

function normal_type(variable){
    let atype = Object.prototype.toString.call(variable);  //  "[object Null]"
    switch(atype){
        case '[object Null]':
            return 'null'
        case '[object Array]':
            return 'array'
        case '[object Object]':
            return 'object'
        case '[object Number]':
            return 'number'
        case '[object String]':
            return 'string'
        case '[object Boolean]':
            return 'boolean'
        case '[object Undefined]':
            return 'undefined'
        }
}

module.exports = {
    fast_type,
    normal_type
}