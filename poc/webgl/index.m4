include(`he.m4')dnl
<!doctype html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Javascript 3D Model Viewer</title>
    <style>
      #viewer {
	  width: 60%;
	  height: 400px;
      }
    </style>
  </head>

  <body>
    <div class="content">
      <div id="viewer" style="">
      </div>

      <div class="center">
	<span id="name">Cell</span>
	<span id="loading"></span>
      </div>

      <div class="center">
	<button class="button" id="fullscreen-button">
	  Full Screen
	</button>
      </div>
    </div>

    <script src="./dist/js-3d-model-viewer.js"></script>
    <script>
include(`index.js')dnl
    </script>
  </body>
</html>
