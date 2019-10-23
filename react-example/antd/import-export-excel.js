/*
 * 在 React + Antd 环境中导入 / 导出 Excel
 *
 * 依赖库：https://github.com/SheetJS/js-xlsx
 * 参阅文章：https://juejin.im/post/5d80707bf265da039135494b
 */

import React from 'react';
import ReactDOM from "react-dom";
import 'antd/dist/antd.css';
import {Button, Upload, Icon, Table, Layout} from 'antd';
import XLSX from 'xlsx';
const { Content } = Layout;

class App extends React.Component {
  state = {
    columns: [],  // Table 列标题
    data: [],  // Table 数据源
  };

  beforeUpload = (file, fileList) => {
    /*
     * 选择本地文件，读取其内容并转换成工作簿格式
     *
     * 参考 Parsing Workbooks 中的 Browser file upload form element 部分
     * https://github.com/SheetJS/js-xlsx#parsing-workbooks
     */
    let reader = new FileReader();
    reader.onload = (e) => {
      let workbook = XLSX.read(new Uint8Array(e.target.result), {type: 'array'});

      /*
       * 工作簿中含有多个表格，提取默认表格（第 0 个，一般只用它来存数据）
       *
       * 参考 Working with the Workbook 中的 Reading a specific cell
       * https://github.com/SheetJS/js-xlsx#working-with-the-workbook
       */
      let worksheet = workbook.Sheets[workbook.SheetNames[0]];

      /*
       * 将工作表转换成 JSON 数据格式
       *
       * 参考 sheet_to_json 方法的使用
       * https://github.com/SheetJS/js-xlsx#json
       */
      let data = XLSX.utils.sheet_to_json(worksheet);

      /*
       * 构建 Table 列标题列表
       *
       * 为了方便起见，直接遍历一个 Object 的键列表即可
       */
      let tmpColumns = [];
      for (let key in data[0]) {
        tmpColumns.push({title: key, dataIndex: key, key: key})
      }

      // 更新 Table 列标题列表和数据源
      this.setState({
        columns: tmpColumns,
        data: data,
      });
    };

    reader.readAsArrayBuffer(file);

    // 阻止 Upload 组件的默认上传行为
    return false;
  };

  exportWorkBook = () => {
    let data = this.state.data;
    // 因为 Sheet 转 JSON 时默认会增加多余的 key 字段，故此处将其删除
    // 以便导出 Excel 时格式正确
    for (let i = 0; i < data.length; ++i) {
      delete data[i]['key']
    }

    // JSON 转 WorkSheet（工作表）
    let worksheet = XLSX.utils.json_to_sheet(data);

    // 创建 WorkBook（工作簿）
    let workbook = XLSX.utils.book_new();

    // 将 工作表 添加进 工作簿
    XLSX.utils.book_append_sheet(workbook, worksheet, '工作簿1');

    // 导出工作簿
    // 文件名使用当前时间指定
    XLSX.writeFile(workbook, Date.now() + '.xlsx');
  };

  render() {
    return (
      <Layout>
        <Content>
          <Upload accept='.xlsx' beforeUpload={this.beforeUpload}>
            <Button><Icon type='upload'/>导入 Excel</Button>
          </Upload>
          <Button onClick={this.exportWorkBook}><Icon type='export'/>导出 Excel</Button>
          <Table columns={this.state.columns} dataSource={this.state.data}/>
        </Content>
      </Layout>
    )
  }
}

ReactDOM.render(<App />, document.getElementById('root'));