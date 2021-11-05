<template>
  <div>
    <div class="top">
      <PositionSelect ref="positionSelect" :click-func="onClickFunc" />
    </div>
    <div class="left">
      <div style="display: inline;float: none">
        <el-card class="numDetail">
          <span style="display: block">全国中风险</span>
          <span class="num" style="color: greenyellow">{{ this.levelSum[0] }}</span>
          <span style="display: block;text-align: right">例</span>
        </el-card>
        <el-card class="numDetail">
          <span style="display: block">全国高风险</span>
          <span class="num" style="color: darkred">{{ this.levelSum[1] }}</span>
          <span style="display: block;text-align: right">例</span>
        </el-card>
      </div>
      <el-card class="chart"  >
        <span style="display: block;margin-bottom: 30px">全国风险地区统计</span>
        <detail-pie-graph ref="detailPieGraph"  style="float: left;width: 70%"/>
        <map-detail ref="mapDetail" style="float: left ;width: 30%"/>
      </el-card>
    </div>
    <div class="right" >
      <el-card >
        <RiskAreaDetail  :detail="tableData" />
      </el-card>
    </div>
  </div>
</template>

<script>
import PositionSelect from "../components/PositionSelect";
import DetailPieGraph from "../components/DetailPieGraph";
import MapDetail from "../components/MapDetail";
import RiskAreaDetail from "../components/RiskAreaDetail";
export default {
  name: "RiskArea",
  components: {RiskAreaDetail, DetailPieGraph, PositionSelect,MapDetail},
  data(){
    return{
      riskAreaSet:[],
      pieData:[],
      levelSum:[0,0],
      tableData:[]
    }
  },
  methods:{
    buildPieChart(){
      this.pieData = [];
      let nameMap = new Map();
      for(let item of this.riskAreaSet){
        if(nameMap.has(item.province)){
          nameMap.set(item.province,nameMap.get(item.province)+1);
        }
        else{
          nameMap.set(item.province,1);
        }
      }
      for(let key of nameMap.keys()){
        this.pieData.push({
          name : key,
          value : nameMap.get(key)
        })
      }
      this.$refs.detailPieGraph.buildPie(this.pieData);
      this.$refs.mapDetail.buildMapDetail(this.pieData);
    },
    getData(){
      this.$axios.get('/risk_areas').then(
          (res)=>{
            for(let item of res.data){
              let getProvince = item.Address.match(/.+?(省|市|自治区|自治州|县|区)/g);
              this.riskAreaSet.push({
                province: getProvince[0],
                city: getProvince[1],
                country: getProvince[2],
                address: item.Address,
                level: item.RiskLevel === '1'? '中' : '高'
              });
              this.tableData.push({
                address: item.Address,
                level: item.RiskLevel === '1'? '中' : '高'
              })
              if(item.RiskLevel === '1'){
                this.levelSum[0]++;
              }
              else if(item.RiskLevel ==='2'){
                this.levelSum[1]++;
              }
            }
            this.buildPieChart();
          }
      )
    },
    onClickFunc(){
      this.tableData = [];
      for(let item of this.riskAreaSet){
        if(item.province === this.$refs.positionSelect.province){
          if(item.city === this.$refs.positionSelect.city){
            if(item.country === this.$refs.positionSelect.country){
              this.tableData.push({
                address: item.address,
                level: item.level
              })
            }
          }
        }
      }
    }
  },
  mounted() {
    this.levelSum = [0,0];
    this.riskAreaSet = [];
    this.tableData = [];
    this.getData();
  }
}
</script>

<style scoped>
.left{
  float: left;
  margin-top: 20px;
  width: 49%;
}
.right{
  float: right;
  margin-top: 20px;
  width: 49%;
}
.numDetail{
  width: 49%;
  float: left;
  margin-left: 4px;
  margin-bottom: 30px;
}
.chart{
  display: block;
  float: none;
  width: 100%;
  height: 400px;
}
.num{
  display: block;
  text-align: center;
  font-size: 50px;
}
</style>