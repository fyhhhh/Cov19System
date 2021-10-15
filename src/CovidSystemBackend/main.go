package main

import (
   "fmt"
   "github.com/gin-gonic/gin"
   "gorm.io/driver/mysql"
   "gorm.io/gorm"
)
type Hospital struct {
   Id       int32
   Province string
   City     string
   District string
   Address  string
   Title    string
   Tel      string
   Location string
}
func main(){
   dsn := "root:156324@(localhost:3306)/cov19?charset=utf8mb4&parseTime=True&loc=Local"
   db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
   if err != nil{
      panic(err)
   }
   if db == nil {
      fmt.Println("DB open failed")
      return
   }else{
      fmt.Println("Database Cov19 open success")
   }



   r := gin.Default()
   r.GET("/hospital", func(c *gin.Context) {
      var hospitals []Hospital
      result := db.Find(&hospitals)
      if result.Error != nil{
         fmt.Println("Hospitals select failed")
      }
      /*for i,s:=range hospitals{
         fmt.Printf("%d is %#v\n",i,s)
      }*/
      c.JSON(200, hospitals)
   })
   r.Run() // 监听并在 0.0.0.0:8080 上启动服务
}