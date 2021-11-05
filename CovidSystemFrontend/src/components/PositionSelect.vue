<template>
<el-card style="text-align: center;margin-top: 5px;width: 100%">
  <i class="el-icon-search" style="padding-left: 10px" />
  <el-select class="select"  v-model="province" placeholder="省级" filterable @change="selectProvince($event)">
    <el-option v-for="(item,key) in provinceSet" :key="key" :label="item.value" :value="item.value" ></el-option>
  </el-select>
  <el-select class="select" v-model="city" placeholder="市级" filterable @change="selectCity($event)">
    <el-option v-for="(item,key) in citySet" :key="key" :label="item.value" :value="item.value"></el-option>
  </el-select>
  <el-select class="select" v-model="country" filterable placeholder="县级">
    <el-option v-for="(item,key) in countrySet" :key="key" :label="item.value" :value="item.value"></el-option>
  </el-select>
  <el-button @click="onClick" type="primary" round>查询</el-button>
</el-card>
</template>

<script>
export default {
  name: "PositionSelect",
  props: ['clickFunc'],
  data(){
    return{
      provinceSet: [],
      citySet: [],
      countrySet: [],
      province : '',
      city : '',
      country: ''
    }
  },
  methods:{
    selectProvince(){
      this.city = '';
      this.country = '';
      this.countrySet = [];
      this.loadCitySet();
    },
    selectCity(){
      this.country = '';
      this.countrySet = [];
      this.loadCountrySet();
    },
    onClick(){
      this.clickFunc();
    },
    loadProvinceSet(){
      this.provinceSet = [];
      this.$axios.get('/area',{params:{province : 'all'}}).then(
        (res) => {
          for(let item of res.data){
            this.provinceSet.push({value : item});
          }
        }
      )
    },
    loadCitySet(){
      this.citySet = [];
      this.$axios.get('/area',{params:{province : this.province, city : 'all'}}).then(
          (res) => {
            for(let item of res.data){
              this.citySet.push({value : item});
            }
          }
      )
    },
    loadCountrySet(){
      this.countrySet = [];
      this.$axios.get('/area',{params:{city : this.city}}).then(
          (res) => {
            for(let item of res.data){
              this.countrySet.push({value : item});
            }
          }
      )
    }
  },
  mounted() {
    this.loadProvinceSet();
  }
}
</script>

<style scoped>
.select{
  margin-left: 5px;
  margin-right: 15px;
}
</style>