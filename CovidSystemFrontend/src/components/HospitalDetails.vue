<template>
  <div>
  <el-button type="text" @click="centerDialogVisible = true">详情</el-button>
  <el-dialog title="详情" :visible.sync="centerDialogVisible" width="30%" center destroy-on-close>
    <el-card class="detail-card">
      <span>医院名称：{{this.details.title}}</span>
    </el-card>
    <el-card class="detail-card">
      <span>医院地址：{{this.details.address}}</span>
    </el-card>
    <el-card class="detail-card">
      <span>医院电话：{{this.details.tel}}</span>
    </el-card>
  </el-dialog>
  </div>
</template>

<script>
export default {
  name: "HospitalDetails",
  props:['details'],
  data(){
    return{
      centerDialogVisible:false
    }
  }
}
</script>

<style scoped>
.detail-card{
  margin-bottom: 15px;
}
</style>