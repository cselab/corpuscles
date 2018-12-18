var modelPlayer = this['js-3d-model-viewer']
var nameElement = document.getElementById('name')
var loadingElement = document.getElementById('loading')
var viewerElement = document.getElementById('viewer')
var scene = modelPlayer.prepareScene(viewerElement)

viewerElement.addEventListener('loading', function (event) {
    if (event.detail.loaded === 0) {
	nameElement.innerHTML = 'Loading...'
    }
    var progress = Math.floor(100 * event.detail.loaded / event.detail.total)
    loadingElement.innerHTML = progress + '%'
})

modelPlayer.loadObject(scene, './assets/sample.obj', null, function () {
    nameElement.innerHTML = 'Cell'
    loadingElement.innerHTML = ''
})

var fullScreenButton = document.getElementById('fullscreen-button')
fullScreenButton.addEventListener('click', function () {
    modelPlayer.goFullScreen(viewerElement)
})

var sample1Button = document.getElementById('sample-1-button')
sample1Button.addEventListener('click', function () {
    modelPlayer.clearScene(scene)
    modelPlayer.resetCamera(scene)
    modelPlayer.loadObject(scene, './assets/sample.obj', null, function () {
	nameElement.innerHTML = 'Sample 01'
	loadingElement.innerHTML = ''
    })
})
