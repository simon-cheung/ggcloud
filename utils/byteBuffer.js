
let ByteBuffer = function () {
    this.appendByteBuffer=function (byteBuf) {

        byteBuf.offset=0
        var n=byteBuf.count
        this.extendBuffer(n)
        for(var i=0; i<n; i++)
        {
            var a=byteBuf.readUint8()
            this.putUint8(a)
        }
    }

    this.initBlank=function()
    {
        this.buffer= new ArrayBuffer(2048)
        this.view = new DataView(this.buffer)
        this.woffset=0
        this.roffset=0
        this.count=0
        this.length=2048
    }

    this.reuse=function(size)
    {
        if(this.roffset < size)
            return
        this.roffset-=size
    }
    
    this.unused=function(){
        return this.count - this.roffset
    }

    this.splitUsedBuffer=function(){
        for(var i=0; i<this.count - this.roffset; i++){
            this.view[i] = this.view[i+this.roffset]
        }
        this.count -= this.roffset
        this.woffset = this.count
        this.roffset = 0
    }

    this.extendBuffer=function (n) {
        if((this.count+n)>=this.length)
        {
            if(n<200){
                n=200
            }
            var newLength=this.count+n
            var newBuffer=new ArrayBuffer(newLength)
            var view = new Uint8Array(newBuffer)
            var oldview = new Uint8Array(this.buffer)
            for(var  i=0; i<this.count; i++){
                view[i]=oldview[i]
            }
            this.buffer=view.buffer
            this.length=newLength
            this.view = new DataView(this.buffer)
        }
    }

    this.appendMsg=function (message) {
        // Discard empty messages
        if (message.length == 0) {
            return
        }
        var length = message.length
        for (var i = 0; i < length; i++) {
            this.putUint8(message[i])
        }
    }

    this.readUint8=function () {
        var a=this.view.getUint8(this.roffset)
        this.roffset+=1
        return a
    }

    this.readInt=function () {
        var a=this.view.getInt32(this.roffset,true)
        this.roffset+=4
        return a
    }
    this.readShort=function () {
        var a=this.view.getInt16(this.roffset,true)
        this.roffset+=2
        return a
    }
    this.readByte=function () {
        var a=this.view.getInt8(this.roffset)
        this.roffset+=1
        return a
    }

    this.readInt8=function () {
        var a=this.view.getInt8(this.roffset)
        this.roffset+=1
        return a
    }

    this.readInt16=function () {
        var a=this.view.getInt16(this.roffset,true)
        this.roffset+=2
        return a
    }
    this.readInt32=function () {
        var a=this.view.getInt32(this.roffset,true)
        this.roffset+=4
        return a
    }
    this.readLongType=function () {
        return this.readInt32()
    }
    this.readUTF=function () {
        var len=this.view.getInt16(this.roffset,true)
        this.roffset+=2
        var str=""
        for(var i=0; i<len; i++)
        {
            var charCode = this.view.getUint16(this.roffset,true)
            this.roffset+=2
            str += String.fromCharCode(charCode)

        }
        return str
    }
    this.readFloat32=function () {
        var a=this.view.getFloat32(this.roffset,true)
        this.roffset+=4
        return a
    }
    this.readFloat=function () {
        var a=this.view.getFloat32(this.roffset,true)
        this.roffset+=4
        return a
    }
    this.readFloat64=function () {
        var a=this.view.getFloat64(this.roffset,true)
        this.roffset+=8
        return a
    }
    this.readDouble=function () {
        var a=this.view.getFloat32(this.roffset,true)
        this.roffset+=4
        return a
    }

    this.putByte=function (a) {
        this.extendBuffer(1)
        this.view.setInt8(this.woffset,a)
        this.woffset+=1
        this.count+=1
    }

    this.putShort=function (a) {
        this.extendBuffer(2)
        this.view.setInt16(this.woffset,a,true)
        this.woffset+=2
        this.count+=2
    }
    this.putInt=function (a) {
        this.extendBuffer(4)
        this.view.setInt32(this.woffset,a,true)
        this.woffset+=4
        this.count+=4
    }

    this.putInt8=function (a) {
        this.extendBuffer(1)
        this.view.setInt8(this.woffset,a)
        this.woffset+=1
        this.count+=1
    }
    this.putInt16=function (a) {
        this.extendBuffer(2)
        this.view.setInt16(this.woffset,a,true)
        this.woffset+=2
        this.count+=2
    }
    this.putInt32=function (a) {
        this.extendBuffer(4)
        this.view.setInt32(this.woffset,a,true)
        this.woffset+=4
        this.count+=4
    }
    this.putLongType=function (a) {
        this.putInt32(a)
    }
    this.putFloat=function (a) {
        this.extendBuffer(4)
        this.view.setFloat32(this.woffset,a,true)
        this.woffset+=4
        this.count+=4
    }

    this.putFloat32=function (a) {
        this.extendBuffer(4)
        this.view.setFloat32(this.woffset,a,true)
        this.woffset+=4
        this.count+=4
    }
    this.putFloat64=function (a) {
        this.extendBuffer(8)
        this.view.setFloat64(this.woffset,a,true)
        this.woffset+=8
        this.count+=8
    }
    this.putUTF=function (a) {

        if(a==undefined)
        {
            a=""
        }
        var len=a.length
        this.putInt16(len)
        this.extendBuffer(len*2)
        for(var i=0; i<len; i++)
        {
            this.view.setUint16(this.woffset,a.charCodeAt(i),true)
            this.woffset+=2
            this.count+=2
        }

    }
    this.putDouble=function (a) {
        this.extendBuffer(4)
        this.view.setFloat32(this.woffset,a,true)
        this.woffset+=4
        this.count+=4
    }
    this.putUint8=function (a) {
        this.extendBuffer(1)
        this.view.setUint8(this.woffset,a)
        this.woffset+=1
        this.count+=1
    }
    this.putUint8Array=function (arr) {
        for(var i=0; i < arr.length; i++){
            this.putUint8(arr[i])
        }
    }
    this.readUint8Array=function(len){
        if(this.count - this.roffset < len)
            return null

        var newBuffer=new ArrayBuffer(len)
        var view = new Uint8Array(newBuffer)
        for(var i = 0; i < len; i++){
            view[i] = this.readUint8()
        }
        return view
    }
}

module.exports = ByteBuffer