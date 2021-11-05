<template>
  <el-card style="width: 400px;margin: 50px">
    <div slot="header">
      <span>当前信息</span>
      <el-button type="text" round  @click="getLocation" style="float: right; padding: 3px 0">获取定位</el-button>
    </div>
    <el-card class="item" style="border-radius: 50px" shadow="hover">经度<el-divider direction="vertical"></el-divider>{{this.location.longitude}}</el-card>
    <el-card class="item" style="border-radius: 50px" shadow="hover">纬度<el-divider direction="vertical"></el-divider>{{this.location.latitude}}</el-card>
    <el-card class="item" style="border-radius: 50px" shadow="hover">省份<el-divider direction="vertical"></el-divider>{{this.position.province}}</el-card>
    <el-card class="item" style="border-radius: 50px" shadow="hover">城市<el-divider direction="vertical"></el-divider>{{this.position.city}}</el-card>
    <el-card class="item" style="border-radius: 50px" shadow="hover">区县<el-divider direction="vertical"></el-divider>{{this.position.district}}</el-card>
    <el-card class="item" style="border-radius: 50px" shadow="hover">街道<el-divider direction="vertical"></el-divider>{{this.position.detail}}</el-card>
  </el-card>
</template>

<script>
import AMapLoader from '@amap/amap-jsapi-loader';

export default {
  name: "LocationCard",
  data() {
    return {
      location: {
        //经度
        longitude: 0,
        //纬度
        latitude: 0.
      },
      position: {
        province : '',
        city : '',
        district : '',
        detail:''
      }
    }
  },
  methods: {
    getLocation() {
      if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition((position) => {
          this.location.longitude = position.coords.longitude;
          this.location.latitude = position.coords.latitude;
         this.covertIt();
        });
      } else {
        this.$message.error("该浏览器不支持获取地理位置。");
      }
    },
   covertIt() {
      let lnglat = [this.location.longitude,this.location.latitude];
      let log = this.$message.error;
      let ps = this.position;
      let callParent =this.$parent.setAddress;
      AMapLoader.load({
        "key": "ea101ceba5d6b4c0d828806cd985b221",              // 申请好的Web端开发者Key，首次调用 load 时必填
        "plugins": ['AMap.Geocoder']           // 需要使用的的插件列表，如比例尺'AMap.Scale'等
      }).then((AMap) => {
        AMap.plugin('AMap.Geocoder', function () {
          let geocoder = new AMap.Geocoder({});
          geocoder.getAddress(lnglat, function (status, result) {
            if (status === 'complete' && result.info === 'OK') {
              ps.province = result.regeocode.addressComponent.province;
              ps.city = result.regeocode.addressComponent.city;
              ps.district = result.regeocode.addressComponent.district;
              ps.detail = result.regeocode.addressComponent.street;
              callParent(result.regeocode.formattedAddress);
            } else {
              log('根据经纬度查询地址失败');
              return ''
            }
          })
        })
      }).catch(e => {
        console.log(e);
      });
    },
    mounted() {
    }
  }
}
</script>

<style scoped>
.item{
  margin-bottom: 10px;
}
</style>