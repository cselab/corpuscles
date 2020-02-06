var loader, camera, scene, renderer, params, gui

if (THREE.WEBGL.isWebGLAvailable()) {
    gui = new dat.GUI()
    params = { file : '0'}
    gui.add(params, 'file', [ '0', '1', '2' ] )
    gui.open()
    loader = new THREE.FileLoader()
    loader.load('data/rbc.off', load, progress, err)
    var obj = { add:function(){ console.log("clicked") }};
    gui.add(obj,'add');
} else {
	var warning = THREE.WEBGL.getWebGLErrorMessage()
        document.body.appendChild(warning)
}

function load(d)
{
        var X = 0, Y = 1, Z = 2
        var XX = 0, XY = 1, XZ = 2,
            YX = 4, YY = 5, YZ = 6,
            ZX = 8, ZY = 9, ZZ = 10
        var nt, nv, t, tt, i, j, k, off, ver, pos, v
        var x, y, z, ve, t
        var geometry, material, mesh
        var controls
        var fov, aspect, near, far

        off = off_string_ini(d)
        nt = off_nt(off)
        nv = off_nv(off)
        tt = off_tri(off)
        ver = off_ver(off)

        t = [0.97711056,    0.21217471,  -0.015386616,             0,
             -0.085192636,    0.32400259,   -0.94221258,             0,
             -0.19492838,    0.92195672,    0.33466208,             0,
             0,             0,             0,             1]
        for (i = 0; i < nv; i++) {
            ve = ver[i]
            x = ve[X]
            y = ve[Y]
            z = ve[Z]
            ve[X] = x*t[XX] + y*t[YX] + z*t[ZX]
            ve[Y] = x*t[XY] + y*t[YY] + z*t[ZY]
            ve[Z] = x*t[XZ] + y*t[YZ] + z*t[ZZ]
        }

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
        geometry.addAttribute('position', new THREE.Float32BufferAttribute(pos, 3))

        scene = new THREE.Scene()
        material = new THREE.MeshBasicMaterial({color: 0xffffff, wireframe: true})

        mesh = new THREE.Mesh(geometry, material)
        scene.add(mesh)
        renderer = new THREE.WebGLRenderer()
        renderer.setSize(window.innerWidth, window.innerHeight)
        document.body.appendChild(renderer.domElement)

        fov = 30
        aspect = window.innerWidth / window.innerHeight
        near = 0.1
        far = 100
        camera = new THREE.PerspectiveCamera(fov, aspect, near, far)
        camera.position.z = 3

        controls = new THREE.OrbitControls(camera, renderer.domElement)
        controls.update()

        animate()
}

function animate()
{
	requestAnimationFrame(animate)
        renderer.render(scene, camera)
        console.log(params.file)
}


function progress(xhr)
{
}

function err(e)
{
        console.log('error reading off file')
}
