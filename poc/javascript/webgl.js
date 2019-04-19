main()

function main()
{
	var c, g, info, buffers

	c = document.querySelector('#c')
	g = c.getContext('webgl')
	info = iniInof(g)
	iniPostion(g)
	iniIndices(g)
	draw(g, info.program, info)
}

function iniInof(g)
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
			gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
		}
	`
	program = iniProgram(g, v, f)
	info = {
		program: program,
		Vertex: g.getAttribLocation(program, 'Vertex'),
		Projection: g.getUniformLocation(program, 'Projection'),
		View: g.getUniformLocation(program, 'View')
	}
	return info
}

function iniPostion(g)
{
	var b, d

	d = [
		 0, 0, 0,
		 1, 0, 0,
		 0, 1, 0,
		0, -1, 0
	]
	b = g.createBuffer()
	g.bindBuffer(g.ARRAY_BUFFER, b)
	g.bufferData(g.ARRAY_BUFFER, new Float32Array(d), g.STATIC_DRAW)
}

function iniIndices(g)
{
	var b, d

	d = [0, 1, 3, 1, 0, 2]
	b = g.createBuffer()
	g.bindBuffer(g.ELEMENT_ARRAY_BUFFER, b)
	g.bufferData(g.ELEMENT_ARRAY_BUFFER, new Uint16Array(d) , g.STATIC_DRAW)
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

function draw(g, program, info)
{
	var aspect, View, Projection
	var step, type, normalize, offset, step, stride, dim = 3

	aspect = g.canvas.clientWidth/g.canvas.clientHeight
	View = getView()
	Projection = getProjection(aspect)
	g.useProgram(program)
	g.uniformMatrix4fv(info.Projection, false, Projection)
	g.uniformMatrix4fv(info.View, false, View)				 

	step = 6
	type = g.FLOAT
	normalize = false
	stride = 0
	offset = 0
	g.vertexAttribPointer(info.Vertex, dim, type, normalize, stride, offset)
	g.enableVertexAttribArray(info.Vertex)
	offset = 0
	 type = g.UNSIGNED_SHORT
	g.drawElements(g.TRIANGLES, step, type, offset)
}

function iniProgram(g, v, f)
{
	var p
	v = loadShader(g, g.VERTEX_SHADER, v)
	f = loadShader(g, g.FRAGMENT_SHADER, f)
	p = g.createProgram()
	g.attachShader(p, v)
	g.attachShader(p, f)
	g.linkProgram(p)
	if (!g.getProgramParameter(p, g.LINK_STATUS)) {
		alert('Unable to iniialize the shader program: ' + g.getProgramInfoLog(p))
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
