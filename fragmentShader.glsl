#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 vertNorm;
in vec4 vertPos;

// Ouput data
out vec4 color;

uniform float currTex;
uniform float Ns;
uniform vec3 eyeLoc;
uniform vec3 lightLoc;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;
uniform float showTex;
uniform float currElement_F;

uniform sampler2D myTextureSampler0, myTextureSampler1, myTextureSampler2, 
	myTextureSampler3, myTextureSampler4, myTextureSampler5, myTextureSampler6, 
	myTextureSampler7, myTextureSampler8, myTextureSampler9, myTextureSampler10,
	myTextureSampler11, myTextureSampler12, myTextureSampler13, myTextureSampler14,
	myTextureSampler15;

float saturate(float t)
{
	return (t >= 0.0 ? t <= 1.0 ? t : 1.0 : 0.0);
}

void main()
{
	if (currElement_F == 1)
	{
		color = vec4(1.0,1.0,1.0,0.2);
	}
	else if (currElement_F == 2)
	{
		color = texture2D(myTextureSampler0, UV);
	}
	else
	{
		// Output color = color specified in the vertex shader, 
		// interpolated between all 3 surrounding vertices
		vec4 lightVec = normalize(vec4(lightLoc, 1) - vertPos);
		float innProd = saturate(dot(lightVec, vertNorm));
		vec4 viewer = normalize(vec4(eyeLoc, 0) - vertPos);
		vec4 reflected = normalize(2 * saturate(dot(lightVec, vertNorm)) * vertNorm - lightVec);
		//0'1'2'3'4'5'6'7'8'9'10'11'12'13'14'15
		vec3 color1;
		if (currTex <= 7)
		{
			if (currTex <= 3)
			{
				if (currTex <= 1)
				{
					if (currTex == 0)
						color1 = texture2D(myTextureSampler0, UV).rgb;
					else //if(currTex == 1)
						color1 = texture2D(myTextureSampler1, UV).rgb;
				}
				else
				{
					if (currTex == 2)
						color1 = texture2D(myTextureSampler2, UV).rgb;
					else //if(currTex == 3)
						color1 = texture2D(myTextureSampler3, UV).rgb;
				}
			}
			else
			{
				if (currTex <= 5)
				{
					if (currTex == 4)
						color1 = texture2D(myTextureSampler4, UV).rgb;
					else
						color1 = texture2D(myTextureSampler5, UV).rgb;
				}
				else
				{
					if (currTex == 6)
						color1 = texture2D(myTextureSampler6, UV).rgb;
					else //if(currTex == 7)
						color1 = texture2D(myTextureSampler7, UV).rgb;
				}

			}
		}
		else
		{
			if (currTex <= 11)
			{
				if (currTex <= 9)
				{
					if (currTex == 8)
						color1 = texture2D(myTextureSampler8, UV).rgb;
					else
						color1 = texture2D(myTextureSampler9, UV).rgb;
				}
				else
				{
					if (currTex == 10)
						color1 = texture2D(myTextureSampler10, UV).rgb;
					else
						color1 = texture2D(myTextureSampler11, UV).rgb;
				}
			}
			else
			{
				if (currTex <= 13)
				{
					if (currTex == 12)
						color1 = texture2D(myTextureSampler12, UV).rgb;
					else
						color1 = texture2D(myTextureSampler13, UV).rgb;
				}
				else
				{
					if (currTex == 14)
						color1 = texture2D(myTextureSampler14, UV).rgb;
					else
						color1 = texture2D(myTextureSampler15, UV).rgb;
				}
			}
		}

		//vec3 color1 = currTex == 0.0 ? texture2D(myTextureSampler0, UV).rgb : currTex == 1.0 ? texture2D(myTextureSampler1, UV).rgb : texture2D(myTextureSampler2, UV).rgb;
		color.r = Ka.r * La.r + Ld.r * innProd * Kd.r + Ks.r * Ls.r * pow(saturate(dot(reflected, viewer)), Ns) + showTex * color1.r;
		color.g = Ka.g * La.g + Ld.g * innProd * Kd.g + Ks.g * Ls.g * pow(saturate(dot(reflected, viewer)), Ns) + showTex * color1.g;
		color.b = Ka.b * La.b + Ld.b * innProd * Kd.b + Ks.b * Ls.b * pow(saturate(dot(reflected, viewer)), Ns) + showTex * color1.b;
		color.a = 1;
	}
}
