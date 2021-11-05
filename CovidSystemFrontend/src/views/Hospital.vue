<template>
  <div>
<div class="top">
  <PositionSelect ref="positionSelect" :click-func="this.getData" />
</div>
<div class="left">
  <el-card style="width: 100%">
    <el-table :data="tableData" style="width: 100%">
      <el-table-column prop="province" label="省" width="180px"></el-table-column>
      <el-table-column prop="city" label="市" width="180px"></el-table-column>
      <el-table-column prop="district" label="区" width="170px"></el-table-column>
      <el-table-column prop="title" label="医院名称" width="650px"></el-table-column>
      <el-table-column label="操作" width="70px" >
        <template slot-scope="scope">
          <hospital-details :details="scope.row"></hospital-details>
        </template>
      </el-table-column>
    </el-table>
  </el-card>
</div>
</div>
</template>

<script>
import PositionSelect from "../components/PositionSelect";
import HospitalDetails from "../components/HospitalDetails";
export default {
  name: "Hospital",
  components: {HospitalDetails, PositionSelect},
  data(){
    return{
      tableData: []
    }
  },
  methods:{
    getData(){
      this.tableData = [];
      let param = {
        province : this.$refs.positionSelect.province,
        city : this.$refs.positionSelect.city,
        district : this.$refs.positionSelect.country
      };
      this.$axios.get('/hospitals',{params : param}).then(
          (res)=>{
            for(let item of res.data){
              this.tableData.push({
                province: item.Province,
                city: item.City,
                district: item.District,
                address: item.Address,
                title: item.Title,
                tel: item.Tel
              })
            }
          }
      );
    },
    clickFunc(){
      this.getData();
    }
  }
}
</script>

<style scoped>
.left{
  margin-top: 10px;
}
el-table-column{
  text-align: center;
}

</style>