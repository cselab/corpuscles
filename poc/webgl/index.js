var Player = this['js-3d-model-viewer']
var nameElement = document.getElementById('name')
var loadingElement = document.getElementById('loading')
var viewerElement = document.getElementById('viewer')
var scene = Player.prepareScene(viewerElement)

viewerElement.addEventListener('loading', function (event) {
    if (event.detail.loaded === 0) {
	nameElement.innerHTML = 'Loading...'
    }
    var progress = Math.floor(100 * event.detail.loaded / event.detail.total)
    loadingElement.innerHTML = progress + '%'
})

Player.loadObject(scene, './assets/sample.obj', null, function () {
    nameElement.innerHTML = 'Cell'
    loadingElement.innerHTML = ''
})

var fullScreenButton = document.getElementById('fullscreen-button')
fullScreenButton.addEventListener('click', function () {
    Player.goFullScreen(viewerElement)
})

var sample1Button = document.getElementById('sample-1-button')
sample1Button.addEventListener('click', function () {
    Player.clearScene(scene)
    Player.resetCamera(scene)
    Player.loadObject(scene, './assets/sample.obj', null, function () {
	nameElement.innerHTML = 'Sample 01'
	loadingElement.innerHTML = ''
    })
})
