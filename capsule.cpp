#include capsule.h

void capsule::build(float len, float rt, float rb, int nfaces, bool smooth)
{
	const float PI = 3.14159;
	float i, x, y;

	float p = (2 * PI / nfaces);
	P.clear(); T.clear(); N.clear();// set size to zero, just in case
	P.reserve(18); T.reserve(18); N.reserve(18); // reserve space to avoid re-allocations from the calls below

	GsVec t1, t2, t3;
	for (i = 0; i < 2 * PI; i += (2 * PI / nfaces)) {

		//top cap
		float newp = i + p;
		//bottom circle
		x = rt * cos(newp);
		y = rt * sin(newp);
		t1 = GsVec(x, .5f, y);
		P.push_back(t1);

		x = rb * cos(newp);
		y = rb * sin(newp);
		t2 = GsVec(x, -.5f, y);
		P.push_back(t2);

		x = rb * cos(i);
		y = rb * sin(i);
		t3 = GsVec(x, -.5f, y);
		P.push_back(t3);

		GsVec w = normal(t1, t2, t3);  //texture maps for bottom circle
		if (smooth == false) {
			N.push_back(normal(t1, t2, t3));
			N.push_back(normal(t1, t2, t3));
			N.push_back(normal(t1, t2, t3));
		}
		else {
			GsVec w = GsVec(0, 1, 0);
			N.push_back(t1);//Originally all w; same with below
			N.push_back(t2);
			N.push_back(t3);

		}
		GsVec2 tmap, tmap2, tmap3;

		tmap = GsVec2(-(newp) / (2 * PI), 0);
		tmap2 = GsVec2(-(newp) / (2 * PI), 1);
		tmap3 = GsVec2(-i / (2 * PI), 1);
		T.push_back(tmap);
		T.push_back(tmap2);
		T.push_back(tmap3);

		//top circle

		x = rt * cos(newp);
		y = rt * sin(newp);
		t1 = GsVec(x, .5f, y);
		P.push_back(t1);
		x = rt * cos(i);
		y = rt * sin(i);
		t2 = GsVec(x, .5f, y);
		P.push_back(t2);
		x = rb * cos(i);
		y = rb * sin(i);
		t3 = GsVec(x, -.5f, y);
		P.push_back(t3);

		tmap = GsVec2(-(newp) / (2 * PI), 0); // texture maps for the top circle
		tmap2 = GsVec2(-i / (2 * PI), 0);
		tmap3 = GsVec2(-i / (2 * PI), 1);
		T.push_back(tmap);
		T.push_back(tmap2);
		T.push_back(tmap3);

		if (smooth == false) {
			N.push_back(normal(t1, t3, t2));
			N.push_back(normal(t1, t3, t2));
			N.push_back(normal(t1, t3, t2));
		}
		else {
			GsVec w = GsVec(0, 1, 0);
			N.push_back(t1);//Originally all w; same with below
			N.push_back(t2);
			N.push_back(t3);

		}


	}

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); T.resize(0); N.resize(0);

}
void capsule::draw(const GsMat& tr, const GsMat& pr, Light& l,  Material& m)
{
	// Draw Lines:
	 //ids start a 1

	

	glUseProgram(prog);	
	glBindVertexArray(va[0]);
	if(instead == true)   //switching textures
		glBindTexture(GL_TEXTURE_2D, _texid);
	else
		glBindTexture(GL_TEXTURE_2D, _texid2);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // Textures
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);					//switching textures...

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	float f[4];
	glUniform3fv ( uniloc[2], 1, l.pos.e );
	glUniform4fv ( uniloc[3], 1, l.amb.get(f) );
	glUniform4fv ( uniloc[4], 1, l.dif.get(f) );
	glUniform4fv ( uniloc[5], 1, l.spe.get(f) );

	glUniform4fv(uniloc[6], 1, m.amb.get(f));
	glUniform4fv(uniloc[7], 1, m.dif.get(f));
	glUniform4fv(uniloc[8], 1, m.spe.get(f));
	glUniform1fv(uniloc[9], 1, &m.sh);
	glUniform1fv(uniloc[11], 1, &dr);


	glDrawArrays(GL_TRIANGLES, 0, _numpoints);
}

