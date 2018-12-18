var Player = this['js-3d-model-viewer']
var nameElement = document.getElementById('name')
var viewer = document.getElementById('viewer')
var scene = Player.prepareScene(viewer)

var fullScreenButton = document.getElementById('fullscreen')
fullScreenButton.addEventListener('click', function () {
    Player.goFullScreen(viewer)
})

var LoadButton = document.getElementById('load')
LoadButton.addEventListener('click', function () {
    Player.loadObject(scene, './assets/sample.obj')
    Player.resetCamera(scene)
})
