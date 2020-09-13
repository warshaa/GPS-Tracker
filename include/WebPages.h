#ifndef WEBPAGES_H_
#define WEBPAGES_H_

const char *Index = R"rawliteral(
  <!DOCTYPE HTML>
  <html dir="rtl" lang="ar">
 <meta charset="utf-8">
   <form name='loginForm'>
    <table width='25%' bgcolor='#D0DFE1' align='center'>
        <tr>
            <td colspan=2>
                <center><font size=4><b>تسجيل الدخول</b></font></center>
                <br>
            </td>
            <br>
            <br>
        </tr>
        <td><input type='text' size=25 name='userid'><br></td>
        <td >اسم المستخدم:</td>

        </tr>
        <br>
        <br>
        <tr>
            <td><input type='Password' size=25 name='pwd'><br></td>
            <td>كلمة المرور:</td>

            <br>
            <br>
        </tr>
        <tr>
            <td align = 'center'><input  type='submit' onclick='check(this.form)' value='دخول'></td>
        </tr>
    </table>
</form>
<script>
    function check(form)
    {
    if(form.userid.value=='admin' && form.pwd.value=='11223344')
    {
    window.open('/JQXXMNWER')
    }
    else
    {
     alert('اسم المستخدم أو كلمة المرور خطأ')/*displays error message*/
    }
    }
</script>;
 </html>
)rawliteral";
const char *JQXXMNWER = R"rawliteral(
<!DOCTYPE HTML>
<html dir="rtl" lang="ar">
 <meta charset="utf-8">



<head>
    <title>xالصفحة الرئيسيةx</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">


</head>

<body>
    <h1 align='right' style="margin-right: 10px;">تحديث الاعدادات</h1>
    <hr>

    <div style="  margin-right: 35px;">
        <div style="float: right !important">
            <form action="/get" target="hidden-form">
                اسم نقطة الاتصال :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <input type="number " name="SSID">
                <input type="submit" value="حفظ" onclick="submitMessage()">
                (الاسم الحالي %M1%)
            </form>
        </div>
    </div>
    <br>
    <br>
    <br>

    <div style="  margin-right: 35px;">
        <div style="float: right !important">
            <form action="/get" target="hidden-form">
            الرمز السري لنقطة الاتصال :  &nbsp; <input type="number " name="PASS">
                <input type="submit" value="حفظ" onclick="submitMessage()">
                (الرمز الحالي %M2%)
            </form>
        </div>
    </div>
    <br>
    <br>
    <br>

    <div style="  margin-right: 35px;">
        <div style="float: right !important">
            <form action="/get" target="hidden-form">
                رقم المركبة :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <input type="number " name="ID">
                <input type="submit" value="حفظ" onclick="submitMessage()">
                (الرقم الحالي %M3%)
            </form>
        </div>
    </div>
    <br>
    <br>
    <br>
    <div style="  margin-right: 35px;">
        <div style="float: right !important">
            <form action="/get" target="hidden-form">
                 عنوان الخادم (URL) :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <input type="url " name="URLx">
                <input type="submit" value="حفظ" onclick="submitMessage()">
                (العنوان الحالي %M4%)
            </form>
        </div>
    </div>
    <br>
    <br>
    <br>
    <hr>
    <div style="text-align: center;">
        <h4 style="color: red;">الرجاء التأكد من صحة البيانات المدخلة و يجب النقر على زر اعادة التشغيل بعد تحديث البيانات</h4>
    <form action="/get">
        <button name="restart" type="submit" >اعادة التشغيل</button>
      </form>


          
    </div>
    <br>
    <br>
    <br>
    <br>
    <hr>
    <br>
    <br>
    <br>
    <br>
    <br>
    <br>
    <br>
    
   
</body>

</html>
 

</html>
  
)rawliteral";

const char *Updatex = R"rawliteral(
<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
   <input type='file' name='update'>
        <input type='submit' value='Update'>
    </form>
    xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 <div id='prg'>progress: 0%</div>
 <script>
  $('form').submit(function(e){
  e.preventDefault();
  var form = $('#upload_form')[0];
  var data = new FormData(form);
   $.ajax({
  url: '/update',
  type: 'POST',
  data: data,
  contentType: false,
  processData:false,
  xhr: function() {
  var xhr = new window.XMLHttpRequest();
  xhr.upload.addEventListener('progress', function(evt) {
  if (evt.lengthComputable) {
  var per = evt.loaded / evt.total;
  $('#prg').html('progress: ' + Math.round(per*100) + '%');
  }
  }, false);
  return xhr;
  },
  success:function(d, s) {
  console.log('success!') 
 },
 error: function (a, b, c) {
 }
 });
 });
 </script>;

)rawliteral";

#endif