var loader
loader = new THREE.FileLoader()
loader.load('data/rbc.off', load, progress, err)

function load(d)
{
	var X = 0, Y = 1, Z = 2
	var nt, t, tt, i, j, k, off, ver, pos, v
	var geometry, scene, material, mesh
	var camera, scene, renderer

	off = off_string_ini(d)
	nt = off_nt(off)
	tt = off_tri(off)
	ver = off_ver(off)
	
	pos = Array(9*nt)

	for (t = v = 0; t < nt; t++) {
		i = tt[t][0]
		j = tt[t][1]
		k = tt[t][2]
		
		pos[v++] = ver[i][X]
		pos[v++] = ver[i][Y]
		pos[v++] = ver[i][Z]

		pos[v++] = ver[j][X]
		pos[v++] = ver[j][Y]
		pos[v++] = ver[j][Z]

		pos[v++] = ver[k][X]
		pos[v++] = ver[k][Y]
		pos[v++] = ver[k][Z]
	}

	geometry = new THREE.BufferGeometry()
	geometry.addAttribute( 'position', new THREE.Float32BufferAttribute(pos, 3) )

	scene = new THREE.Scene()
	material = new THREE.MeshBasicMaterial({color: 0x00ffff})
	mesh = new THREE.Mesh(geometry, material)
	scene.add(mesh)
	renderer = new THREE.WebGLRenderer( { antialias: true } )
	renderer.setSize(window.innerWidth, window.innerHeight )
	document.body.appendChild( renderer.domElement )

	camera = new THREE.PerspectiveCamera(40, window.innerWidth / window.innerHeight, 0.01, 10 )
	camera.position.z = 5
	renderer.render( scene, camera )
}

function progress(xhr)
{
}

function err(e)
{
	console.log('error reading off file')
}
