<template>
  <div id="detail-pie">

  </div>
</template>

<script>
export default {
  name: "DetailPieGraph",
  methods:{
    /**
     * @function 创建数据饼图
     * @param detailsSet [{name : string ,value : number}]
     */
    buildPie(detailsSet){
      let mychart = this.$echarts.init(document.getElementById('detail-pie'));
      let option ={
        tooltip: {
          trigger: 'item'
        },
        series: [
          {
            name: 'Access From',
            type: 'pie',
            radius: ['40%', '70%'],
            center : [ '55%', '43%' ],
            avoidLabelOverlap: false,
            itemStyle: {
              borderRadius: 10,
              borderColor: '#fff',
              borderWidth: 2
            },
            grid: {
              height : '250',
              width : '250',
              top : '0'
            },
            label: {
              show: false,
              position: 'center'
            },
            emphasis: {
              label: {
                show: true,
                fontSize: '40',
                fontWeight: 'bold'
              }
            },
            labelLine: {
              show: false
            },
            data: detailsSet
          }
        ]
      };
      mychart.setOption(option);
    }
  }
}
</script>

<style scoped>
#detail-pie{
  width: 350px;
  height: 350px;
  border: 0;
}
</style>