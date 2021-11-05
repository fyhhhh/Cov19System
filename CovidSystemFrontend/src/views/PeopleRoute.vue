<template>
<div>
  <location-card style="display: inline-block"/>
  <el-card style="float: right;margin: 50px;width: 500px;height: 540px">
    <span>你最近到访的地方为</span>
    <el-card class="item" style="border-radius: 50px;margin-top: 20px;text-align: center" shadow="hover">{{lastAddress}}</el-card>
    <span>该地区的风险等级为</span>
    <el-card class="item" style="border-radius: 50px;margin-top: 20px;text-align: center" shadow="hover">{{lastLevel}}</el-card>
    <span>提示信息</span>
    <el-card class="item" style="border-radius: 50px;margin-top: 20px;text-align: center" shadow="hover">{{tip}}</el-card>
    <span>近14天是否到访过中高风险地区</span>
    <el-card class="item" style="border-radius: 50px;margin-top: 20px;text-align: center" shadow="hover">{{isHigh}}</el-card>
  </el-card>
</div>
</template>

<script>
import LocationCard from "../components/LocationCard";
export default {
  name: "PeopleRoute",
  components: {LocationCard},
  data(){
    return{
      lastAddress : '无数据',
      lastLevel : '无数据',
      tip : '无数据',
      tipSet : ['未见异常，祝您生活顺利','处于中风险地区，请注意日常防护','处于高风险地区，请避免外出','您与患者有过接触，请及时联系相关部门'],
      isHigh : '无数据'
    }
  },
  methods:{
   async setAddress(address){
      this.lastAddress = address;
      let level = this.getRiskLevel(address);
      const convert = new Map(
          [
              [0,'低'],
              [1,'中'],
              [2,'高']
          ]
      );
      this.lastLevel = convert.get(level);
      this.tip = this.tipSet[level];
      if(level !==0){
        this.isHigh = '是';
      }
      else{
        this.isHigh = '否';
      }
    },
    getRiskLevel(address){
      let level = 0;
      let addressDetail = address.match(/.+?(省|市|自治区|自治州|县|区)/g);
      this.$axios.get('/risk_areas').then(
          (res)=>{
            for(let item of res.data){
              let getProvince = item.Address.match(/.+?(省|市|自治区|自治州|县|区)/g);
              if(addressDetail[0]===getProvince[0]){
                if(addressDetail[1]===getProvince[1]){
                  if(addressDetail[2]===getProvince[2]){
                    level = Math.max(level,item.RiskLevel+0);
                  }
                }
              }
            }
          }
      )
      return level;
    }
  }
}
</script>

<style scoped>

</style>