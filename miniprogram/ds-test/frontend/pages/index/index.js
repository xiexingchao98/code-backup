Page({
  data: {
    key: 'xc580963264d72f7a757e0946a63fdex',
  },
  onShow: function () {
    let that = this
    wx.getSetting({
      success: function(res) {
        if (!res.authSetting['scope.userLocation']) {
          wx.authorize({
            scope: 'scope.userLocation',
            success: function () {
              wx.reLaunch({
                url: '/pages/index/index'
              })
            }
          })
        } else {
          wx.getLocation({
            success: function (res) {
              console.log(res)
			  // 1. 获取GPS位置信息
			  // 2. 获取区域编码
			  // 3. 获取区域天气
			  // 4. 发送数据至服务器保存
              that.regeo(res.longitude + ',' + res.latitude).then( (adcode) => that.getWeatherInfo(adcode)).then( (res) =>  that.sendData(res.data.lives[0]) )
            }
          })
        }
      }
    })
  },
  regeo(locations) {
    let that = this
    let promise = new Promise(function (resolve, reject) {
      wx.request({
        method: 'GET',
        url: `https://restapi.amap.com/v3/geocode/regeo?key=${that.data.key}&location=${locations}`,
        success: (res) => { resolve(res.data.regeocode.addressComponent.adcode) },
        fail: () => { reject() }
      })
    })
    return promise
  },
  getWeatherInfo(adcode) {
    let that = this
    let promise = new Promise(function (resolve, reject) {
      wx.request({
        method: 'GET',
        url: `https://restapi.amap.com/v3/weather/weatherInfo?key=${that.data.key}&city=${adcode}&extensions=base`,
        success: (res) => { resolve(res) },
        fail: () => { reject() }
      })
    })
    return promise
  },
  sendData(data) {
    let that = this
    let promise = new Promise(function (resolve, reject) {
      wx.request({
        method: 'POST',
        url: 'http://localhost:8080/weather',
        data: data,
        success: (res) => {
          console.log(res)
        }
      })
    })
    return promise
  }
})