main()

function main()
{
	var c, g, info, buf

	c = document.querySelector('#c')
	g = c.getContext('webgl')
	info = iniInfo(g)
	buf = {}
	buf.Vertex = iniVertex(g)
	buf.Color = iniColor(g)
	iniIndices(g)
	draw(g, info.program, info, buf)
}

function iniInfo(g)
{
	var v, f, info, program

	v = `
		attribute vec4 Vertex, Color;
		uniform mat4 View, Projection;
		varying lowp vec4 vColor;
		void main() {
			gl_Position = Projection*View*Vertex;
			vColor = Color;
		}
	`
	f = `
		varying lowp vec4 vColor;
		void main() {
			gl_FragColor = vColor;
		}
	`
	program = iniProgram(g, v, f)
	info = {
		program: program,
		Vertex: g.getAttribLocation(program, 'Vertex'),
		Color: g.getAttribLocation(program, 'Color'),
		Projection: g.getUniformLocation(program, 'Projection'),
		View: g.getUniformLocation(program, 'View')
	}
	return info
}

function iniVertex(g)
{
	var b, d

	d = [
		 0, 0, 0,
		 1, 0, 0,
		 0, 1, 0,
		 0, 0, 0,
		 1, 0, 0,
		 0, -1, 0,
	]
	b = g.createBuffer()
	g.bindBuffer(g.ARRAY_BUFFER, b)
	g.bufferData(g.ARRAY_BUFFER, new Float32Array(d), g.STATIC_DRAW)
	return b
}

function iniColor(g)
{
	var b, d
	d = [
		 0, 1, 0,
		 0, 1, 0,
		 0, 1, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0
	]
	b = g.createBuffer()
	g.bindBuffer(g.ARRAY_BUFFER, b)
	g.bufferData(g.ARRAY_BUFFER, new Float32Array(d), g.STATIC_DRAW)
	return b
}

function iniIndices(g)
{
	var b, d

	d = [0, 1, 2, 3, 4, 5]
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

function bind(g, b, p)
{
	var type, normalize, offset, stride, dim = 3

	g.bindBuffer(g.ARRAY_BUFFER, b)
	type = g.FLOAT
	normalize = false
	stride = 0
	offset = 0
	g.vertexAttribPointer(p, dim, type, normalize, stride, offset)
	g.enableVertexAttribArray(p)
}

function draw(g, program, info, buf)
{
	var aspect, View, Projection
	var step, type, normalize, offset, step, stride, dim = 3

	aspect = g.canvas.clientWidth/g.canvas.clientHeight
	View = getView()
	Projection = getProjection(aspect)
	g.useProgram(program)
	g.uniformMatrix4fv(info.Projection, false, Projection)
	g.uniformMatrix4fv(info.View, false, View)
	bind(g, buf.Vertex, info.Vertex)
	bind(g, buf.Color, info.Color)

	offset = 0
	step = 6
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
