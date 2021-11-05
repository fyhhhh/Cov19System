<template>
  <el-card class="text item" :body-style="style">
    <i :class="icon"></i>
    <span class="value">
      {{value}}
    </span>
    <span class="header">
      {{name}}
    </span>
  </el-card>
</template>

<script>
export default {
  name: "DetailCard",
  props : ['icon','name','value'],
  data(){
    return{
      style :{
        padding : "10px"
      }
    }
  }
}
</script>

<style scoped>
.text{
  padding: 2px;
  text-align: center;
}
.item{
  margin-left: 10px;
  margin-bottom: 10px;
  width: 210px;
  height: 110px;
}
span{
  display: block;
}
.value{
  font-size: 20px;
  padding-top: 10px;
  padding-bottom: 10px;
}
.header{
  font-size: smaller;
  color: #909399;
}
</style>