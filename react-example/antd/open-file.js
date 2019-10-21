import React from 'react';
import ReactDOM from "react-dom";
import 'antd/dist/antd.css';
import {Upload, Button, Icon} from 'antd';

/*
 * 参考:
 *
 * [Blob] -> https://developer.mozilla.org/en-US/docs/Web/API/Blob
 * [FileReader] -> https://developer.mozilla.org/en-US/docs/Web/API/FileReader
 * [beforeUpload()] -> https://ant.design/components/upload-cn/#API
 */

const props = {
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

ReactDOM.render(
  <Upload {...props}>
    <Button>
      <Icon type='upload' />Select file
    </Button>
  </Upload>,
  document.getElementById("root")
);