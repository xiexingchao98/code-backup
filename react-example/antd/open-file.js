import React from 'react';
import ReactDOM from "react-dom";
import 'antd/dist/antd.css';
import {Upload, Button, Icon, Layout} from 'antd';
const { Header, Footer, Sider, Content } = Layout;

/*
 * 提供两种使用方式
 * 1. 本地读取文件数据
 * 2. 本地上传文件至服务端
 */

/*
 * 1. 本地读取文件数据
 *
 * 参考:
 *
 * [Blob] -> https://developer.mozilla.org/en-US/docs/Web/API/Blob
 * [FileReader] -> https://developer.mozilla.org/en-US/docs/Web/API/FileReader
 * [beforeUpload()] -> https://ant.design/components/upload-cn/#API
 */

const props1 = {
  accept: '.csv',
  beforeUpload: (file, fileList) => {
    console.log(file);
    // 仅支持较新的浏览器，如 chrome 76 及以上
    file.text().then((res) => console.log("file.text() => ", res));

    // 更稳妥的写法，兼容多数浏览器
    let reader = new FileReader();
    reader.onload = (e) => {
      console.log("e.targer.result => ", e.target.result);
      // or
      console.log("reader.result => ", reader.result);
    };
    reader.readAsText(file);

    // 阻止默认的文件上传功能
    return false
  }
};

/*
 * 2. 本地上传文件至服务端
 *
 * 注意打开 console 进行观察输出和网络请求
 */
const props2 = {
  name: "file",
  action: "http://localhost:9090/upload",
};

/*
 * 组件渲染
 */

ReactDOM.render(
  <Layout>
    <Content>
      <Upload {...props1}>
        <Button>
          <Icon type='upload' />读取本地文件
        </Button>
      </Upload>

      <Upload {...props2}>
        <Button>
          <Icon type='upload' />上传本地文件至服务端
        </Button>
      </Upload>
    </Content>
  </Layout>,
  document.getElementById("root")
);