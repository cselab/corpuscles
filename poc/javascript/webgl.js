main()

function main()
{
	var c, g, info, buffers

	c = document.querySelector('#c')
	g = c.getContext('webgl')
	info = initInof(g)
	buffers= initBuffers(g)
	draw(g, info.program, info, buffers)
}

function initInof(g)
{
	var v, f, info, program

	v = `
		attribute vec4 Vertex;
		uniform mat4 View, Projection;
		void main() {
			gl_Position = Projection*View*Vertex;
		}
	`
	f = `
		void main() {
			gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	`
	program = initProgram(g, v, f)
	info = {
		program: program,
		Vertex: g.getAttribLocation(program, 'Vertex'),
		Projection: g.getUniformLocation(program, 'Projection'),
		View: g.getUniformLocation(program, 'View')
	}
	return info
}

function initBuffers(g)
{
	var b, pos
	b = g.createBuffer()
	g.bindBuffer(g.ARRAY_BUFFER, b)
	pos = [
		 1.0,	1.5, 0.0,
		-1.2,	1.0, 0.0,
		 1.0, -1.0, 0.0,
		-1.0, -1.0, 0.0
	]
	g.bufferData(g.ARRAY_BUFFER, new Float32Array(pos), g.STATIC_DRAW)
	return { position: b }
}

function getView()
{
	var View
	View = matrix_create()
	matrix_translate(View, View, [0, 0, -4])
	return View
}

function getProjection(aspect)
{
	var fov, zNear, zFar, Projection
	fov = 45*Math.PI/180	
	zNear = 0.1
	zFar = 100.0
	Projection = matrix_create()
	matrix_perspective(Projection, fov, aspect, zNear, zFar)
	return Projection
}

function draw(g, program, info, buffers)
{
	var aspect, View, Projection
	var numComponents, type, normalize, offset, vertexCount , stride


	aspect = g.canvas.clientWidth/g.canvas.clientHeight
	View = getView()
	Projection = getProjection(aspect)
	g.useProgram(program)
	g.uniformMatrix4fv(info.Projection, false, Projection)
	g.uniformMatrix4fv(info.View, false, View)				 

	numComponents = 3
	type = g.FLOAT
	normalize = false
	stride = 0
	offset = 0
	g.bindBuffer(g.ARRAY_BUFFER, buffers.position)
	g.vertexAttribPointer(
				info.Vertex,
				numComponents,
				type,
				normalize,
				stride,
				offset)
	g.enableVertexAttribArray(info.Vertex)
	offset = 0
	vertexCount = 4
	g.drawArrays(g.TRIANGLE_STRIP, offset, vertexCount)
}

function initProgram(g, v, f)
{
	var p
	v = loadShader(g, g.VERTEX_SHADER, v)
	f = loadShader(g, g.FRAGMENT_SHADER, f)
	p = g.createProgram()
	g.attachShader(p, v)
	g.attachShader(p, f)
	g.linkProgram(p)
	if (!g.getProgramParameter(p, g.LINK_STATUS)) {
		alert('Unable to initialize the shader program: ' + g.getProgramInfoLog(p))
		return null
	}
	return p
}

function loadShader(g, type, source)
{
	var shader
	shader = g.createShader(type)
	g.shaderSource(shader, source)
	g.compileShader(shader)
	if (!g.getShaderParameter(shader, g.COMPILE_STATUS)) {
		alert('An error occurred compiling the shaders: ' + g.getShaderInfoLog(shader))
		g.deleteShader(shader)
		return null
	}
	return shader
}
