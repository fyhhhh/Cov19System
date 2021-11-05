<template>
  <div style="height: 100%">
    <div class="left">
      <el-card class="box-card">
        <add-detail-chart ref="addDetailsChart" ></add-detail-chart>
      </el-card>
      <el-card class="box-card" style="height: 340px;margin-top: 10px" :body-style="this.style">
        <el-button v-for="(value,key) in this.buttonList" :key="key" @click="getPieData(value.dataType);remakeButtonList();value.isSelected=true"  style="margin-right: 15px;margin-left: 14px"  :type="value.isSelected?'primary' : 'info'">{{value.name}}</el-button>
        <detail-pie-graph ref="detailPieGraph"/>
      </el-card>
    </div>
    <div class="mid">
      <show-detials class="card" :details="details"></show-detials>
        <ChinaMapLog class="map" ref="chinaMapLog"></ChinaMapLog>
        <MapDetail :detail-list="detailList" ref="mapDetail" style="display: inline-block;float: left;margin-top: 100px;margin-left: 25px" />
    </div>
  </div>
</template>

<script>
import ChinaMapLog from "../components/ChinaMapLog";
import ShowDetials from "../components/ShowDetials";
import AddDetailChart from "../components/AddDetailChart";
import MapDetail from "../components/MapDetail";
import DetailPieGraph from "../components/DetailPieGraph";
export default {
  name: "MapView",
  components: {DetailPieGraph, MapDetail, AddDetailChart, ShowDetials, ChinaMapLog},
  data() {
    return {
      hearderStyle: {
        margin: '0px'
      },
      details: {
      },
      history: {
        confirmAdd: 1,
        healAdd: 10,
        deadAdd: 10
      },
      historyDetails: {
        time: [],
        totalNow: [],
        totalConfirmAdd: [],
        totalHeal: [],
        totalDead: []
      },
      detailList: [],
      style: {
        padding: '5px'
      },
      buttonList: [
        {name: '新增感染', isSelected: false, dataType: 'TotalConfirmAdd'},
        {name: '累计治愈', isSelected: false, dataType: 'TotalHeal'},
        {name: '累计死亡', isSelected: false, dataType: 'TotalDead'}
      ]
    }
  },
  methods: {
    setDetails(newDetails) {
      this.details = newDetails;
    },
    setHistoryDetails(newHistoryDetails) {
      this.historyDetails = newHistoryDetails;
      this.$refs.addDetailsChart.buildEchart(this.historyDetails);
    },
    setHistory(newHistory) {
      this.history = newHistory;
    },
    setDetailList(newDetailList) {
      this.detailList = newDetailList;
      this.$refs.mapDetail.buildMapDetail(this.detailList);
    },
    setDetailPieGraph(newDetailPieGraph) {
      this.$refs.detailPieGraph.buildPie(newDetailPieGraph);
    },
    remakeButtonList() {
      for (let item of this.buttonList) {
        item.isSelected = false;
      }
    },
    getPieData(type) {
      let detailsSet = [];
      for (let item of this.$refs.chinaMapLog.dataList) {
        if (item[type] === 0)
          continue;
        detailsSet.push({
          name: this.$refs.chinaMapLog.selectedMap === 'china' ? item.Province : item.City,
          value: item[type]
        });
      }
      if (detailsSet.length === 0) {
        this.$message.error('地区该项数据全部为零！')
      }
      this.setDetailPieGraph(detailsSet);
    },
  initButton(){
    this.getPieData(this.buttonList[0].dataType);
    this.remakeButtonList();
    this.buttonList[0].isSelected=true;
  }
  }
}
</script>

<style scoped>
add-detail-chart {
  display: inline-block;
  float: left;
}
.left{
  width: 30%;
  float: left;
}
.mid{
  width: 70%;
  float: left;
}
.map{
  display: block;
  margin-top: 10px;
  margin-left: 30px;
  width: 100%;
  float: left;
}
.card {
  display: block;
  margin: 0 auto;
  width: 100%;
}
</style>