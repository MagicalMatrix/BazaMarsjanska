
//convert vector to direction
half3 ToDirection(half3 data)
{
	return data * max(abs(data.x), max(abs(data.y), abs(data.z)));
}

float PseudoRandom(float Seed)
{
	float result = frac(sin(dot(Seed, float3(12.9898, 78.233, 45.5432))) * 43758.5453);
	return result;
}

float PseudoRandom(float2 Seed)
{
	float result = frac(sin(dot(Seed, float3(12.9898, 78.233, 45.5432))) * 43758.5453);
	return result;
}

float2 PseudoRandom2D(float2 p)
{
	return frac(sin(float2(dot(p, float2(127.1, 311.7)), dot(p, float2(269.5, 183.3)))) * 43758.5453);
}

/*
float2 SampleCrossNeighbour(int id, int size, float2 uv, float2 move)
{
	if (id == 0)
	{

	}
	else if (id == 1)
	{

	}
	else if (id == 2)
	{

	}
	else if (id == 3)
	{

	}

	return uv;
}
*/

float2 SampleNeighbour(int id, float2 uv, float2 move)
{
	if (id == 0)
	{
		return uv;
	}
	else if (id == 1)
	{
		return float2(uv.x - move.x, uv.y + move.y);
	}
	else if (id == 2)
	{
		return float2(uv.x, uv.y + move.y);
	}
	else if (id == 3)
	{
		return float2(uv.x + move.x, uv.y + move.y);
	}
	else if (id == 4)
	{
		return float2(uv.x + move.x, uv.y);
	}
	else if (id == 5)
	{
		return float2(uv.x + move.x, uv.y - move.y);
	}
	else if (id == 6)
	{
		return float2(uv.x, uv.y - move.y);
	}
	else if (id == 7)
	{
		return float2(uv.x - move.x, uv.y - move.y);
	}
	else if (id == 8)
	{
		return float2(uv.x - move.x, uv.y);
	}

	return uv;
}

half4 ColorBlend(half4 bgCol, half4 adCol)
{
	half4 result = fixed4(1, 1, 1, 1);
	result.a = 1 - (1 - adCol.a) * (1 - bgCol.a);
	result.rgb = adCol.rgb * adCol.a / result.a + bgCol.rgb * bgCol.a * (1 - adCol.a) / result.a;

	return result;
}

half4 ColorBlend(half3 bgCol, half bgA, half3 adCol, half adA)
{
	half4 result = fixed4(1, 1, 1, 1);
	result.a = 1 - (1 - adA) * (1 - bgA);
	result.rgb = adCol.rgb * adA / result.a + bgCol.rgb * bgA * (1 - adA) / result.a;

	return result;
}

//HUE

half4 ColorToHue6(fixed4 col)
{

	half4 result = half4(0, 0, 0, col.a);
	//value //brightness
	result.y = min(col.x, min(col.y, col.z));
	//saturation //intensity
	result.z = max(col.x, max(col.y, col.z)) - result.y;

	//PRIMARY COLOR
	//blue is always giving 0, so let's just skipp it
	//matrix: b = 1 => 0 g = 1 => 1 r = 1 => 2
	result.x = floor((col.y - result.y) / result.z) * 1 + floor((col.x - result.y) / result.z) * 2;
	//excludation (weird thing, gives 1 for 2, 2 for 4, and 2 for 6 input)
	result.x = min(result.x, 2);

	//SECONDARY COLOR (fixed)
	//int Translation = 
	//Mathf.Ceil(col.y - result.y)
	result.w = ((col.x - result.y) / result.z * min(2 - result.x, 1) + ((col.y - result.y) / result.z + 1 * ceil(col.y - result.y)) * abs(result.x - 1) + ((col.z - result.y) / result.z + 2 * ceil(col.z - result.y)) * min(result.x, 1) + result.x) % 3;

	//result.w += Mathf.Floor((3 - result.w) / 3);
	//1 * Mathf.Floor((3 - result.w) / 3)

	//result.w = (result.w - 1) * Mathf.Min(Mathf.Ceil(col.y - result.y) + Mathf.Ceil(col.z - result.y), 1) + 1;

	//float heck = (result.w - 1) * Mathf.Min(Mathf.Ceil(col.y - result.y) + Mathf.Ceil(col.z - result.y), 1) + 1;

	float Exceptions = ceil(col.x - result.y) + ceil(col.y - result.y) + ceil(col.z - result.y);
	if (Exceptions == 0)
	{
		result.x = 0;
	}
	else if (Exceptions == 1)
	{
		result.x = result.x * 2 + 1;
	}
	else
	{
		result.x = result.x * 2 + abs(ceil(result.x * 2 + result.w) % 2 - result.w);
	}

	result.w = col.a;

	return result;
}

half4 HueToColor6(half4 hue)
{
	hue.x %= 6;
	if (hue.x == 0)
	{
		hue.x = 6;
	}
	//START

	//PrimaryColor:
	/*
	result.x = Mathf.Min(Mathf.Floor(hue.x / 4), 1) * (hue.y + hue.z)
	result.y = Mathf.Min(hue.x / 2, 1) * (1 - Mathf.Min(hue.x / 4, 1)) * (hue.y + hue.z)
	result.z = Mathf.Min(hue.x / 4, 1) * (hue.y + hue.z)
	*/

	half4 result = half4(0, 0, 0, hue.w);

	//OTHERIDEA
	result.w = min(floor(hue.x / 2), 2);
	result.x = max(result.w - 1, 0) * hue.z;
	result.y = (1 - abs(result.w - 1)) * hue.z;
	result.z = -min(result.w - 1, 0) * hue.z;

	//SECONDARY COLOR
	//multiplication Mathf.Ceil(hue.x + 1) % 2;

	result.w = floor(hue.x) % 3 + 2 * floor(hue.x / 6);
	float LittleValue = abs(1 * ceil(hue.x) % 2 - (hue.x - floor(hue.x)));
	//LittleValue = 1 - LittleValue
	if (LittleValue == 1 || LittleValue == 0)
	{
		LittleValue = 1 - LittleValue;
	}
	result.x += -min(result.w - 1, 0) * LittleValue * hue.z;
	result.y += (1 - abs(result.w - 1)) * LittleValue * hue.z;
	result.z += max(result.w - 1, 0) * LittleValue * hue.z;

	result.x += hue.y;
	result.y += hue.y;
	result.z += hue.y;
	result.w = hue.w;

	return result;
}