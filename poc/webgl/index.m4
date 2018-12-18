include(`he.m4')dnl
<!doctype html>
<html>
  <head>
    <meta charset="utf-8">
    <title>he model viewer</title>
    <style>
      #viewer {
	  width: 60%;
	  height: 400px;
      }
    </style>
  </head>

  <body>
    <div class="content">
      <div id="viewer">
      </div>
      <button class="button" id="load">
	Show
      </button>
      <button class="button" id="fullscreen">
	Full Screen
      </button>
    </div>

    <script src="./dist/js-3d-model-viewer.js"></script>
    <script>
include(`index.js')dnl
    </script>
  </body>
</html>
