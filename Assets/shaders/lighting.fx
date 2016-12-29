
float attenuation( float distance, float range, float a, float b, float c)
{
    float atten = 1.0f / ((a * distance * distance) + (b * distance) + c );

    //step clamps to 0 if out of range
    return step(distance, range) * saturate( atten );
}


float3 lightingP(float3 pos, float3 base, float3 normal, float3 lightPos, float3 _camPos ){

	// Lighting vectors
	float3 lVec = (lightPos - pos);
	float3 lightVec = normalize(lVec);
	float3 viewVec = normalize(pos - _camPos);

	// Attenution that falls off to zero at light radius
	const float a = 0.015;
	float atten = 2 * saturate((1.0 + a) / (1.0 + (1.0 / a) * dot(lVec, lVec)) - a);
  atten = attenuation( length(lVec), 4, 1.5,0.01,0.01);

	// Lighting
	float diffuse = saturate(dot(lightVec, normal));
	float specular = 0.2 * pow(saturate(dot(reflect(-viewVec, normal), lightVec)), 10.0);

//return (diffuse * base + specular);
	return atten * (diffuse * base + specular);
//return diffuse * (base );
}

