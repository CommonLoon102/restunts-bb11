/* Included after readonly uint shadowStatic[] and shadowFrame[] SSBOs.
 * Matches the private packed-word ABI in shape3d_shadows.c. All offsets are
 * uint32 words except the explicitly byte-addressed lightmap mip offsets.
 * Static header: faces, face count, nodes, node count, indices, pages,
 * page count, hash table, hash capacity, edges. Face stride20, node12,
 * page4, edge5, texture28. No float64 GPU capability is required.
 * Frame header: active, dynamic, camera.xyz, map.xy, light low.xy/high.xy,
 * contact low.xyz/high.xyz, light rectangle(x,y,w,h,offset), contact rectangle.
 */
const uint SHADOW_NONE = 0xffffffffu;

float shadowValue(uint offset)
{
    return uintBitsToFloat(shadowStatic[offset]);
}
float shadowFrameValue(uint offset)
{
    return uintBitsToFloat(shadowFrame[offset]);
}
vec3 shadowVector(uint offset)
{
    return vec3(shadowValue(offset), shadowValue(offset + 1u), shadowValue(offset + 2u));
}
vec3 shadowFrameVector(uint offset)
{
    return vec3(shadowFrameValue(offset), shadowFrameValue(offset + 1u), shadowFrameValue(offset + 2u));
}
float shadowByte(uint base, uint index)
{
    return float((shadowStatic[base + (index >> 2u)] >> ((index & 3u) * 8u)) & 255u);
}
float shadowLevel(uint pixels, uint byteOffset, uvec2 size, vec2 point)
{
    point = clamp(point, vec2(0.0), vec2(size - 1u));
    uvec2 first = uvec2(point);
    uvec2 next = min(first + 1u, size - 1u);
    vec2 fraction = point - vec2(first);
    float a = shadowByte(pixels, byteOffset + first.y * size.x + first.x);
    float b = shadowByte(pixels, byteOffset + first.y * size.x + next.x);
    float c = shadowByte(pixels, byteOffset + next.y * size.x + first.x);
    float d = shadowByte(pixels, byteOffset + next.y * size.x + next.x);
    float top = a * (1.0 - fraction.x) + b * fraction.x;
    float bottom = c * (1.0 - fraction.x) + d * fraction.x;
    return top * (1.0 - fraction.y) + bottom * fraction.y;
}
uint shadowTexture(uint texture, vec2 point, float footprint)
{
    uint levels = shadowStatic[texture + 2u];
    if (levels == 0u) {
        return 0u;
    }
    uvec2 size = uvec2(shadowStatic[texture], shadowStatic[texture + 1u]);
    float texel = shadowValue(texture + 6u);
    float inverseTexel = shadowValue(texture + 7u);
    point = (point - vec2(shadowValue(texture + 4u), shadowValue(texture + 5u))) * inverseTexel;
    if (any(lessThan(point, vec2(0.0))) || any(greaterThan(point, vec2(size - 1u)))) {
        return 0u;
    }
    uint uniformShade = shadowStatic[texture + 3u];
    if (uniformShade != SHADOW_NONE) {
        return uniformShade;
    }
    uint level = 0u;
    while (footprint > texel * 2.0 && level + 1u < levels) {
        point = (point - 0.5) * 0.5;
        footprint *= 0.5;
        size = (size + 1u) / 2u;
        level++;
    }
    uint pixels = shadowStatic[texture + 8u];
    float shade = shadowLevel(pixels, shadowStatic[texture + 9u + level], size, point);
    if (footprint > texel && level + 1u < levels) {
        float coarse = shadowLevel(pixels, shadowStatic[texture + 10u + level],
                                   (size + 1u) / 2u, (point - 0.5) * 0.5);
        shade += (coarse - shade) * (footprint * inverseTexel - 1.0);
    }
    return uint(shade + 0.5);
}
bool shadowMatches(uint index, vec3 point, out float distance)
{
    uint face = shadowStatic[0] + index * 20u;
    vec3 low = shadowVector(face + 4u);
    vec3 high = shadowVector(face + 7u);
    uint uAxis = shadowStatic[face + 10u];
    uint vAxis = shadowStatic[face + 11u];
    uint axis = shadowStatic[face + 12u];
    if (shadowStatic[face + 13u] != 0u) {
        distance = abs(point[axis] - low[axis]);
        if (!(distance <= 2.0 && point[uAxis] >= low[uAxis] - 2.0 &&
              point[uAxis] <= high[uAxis] + 2.0 && point[vAxis] >= low[vAxis] - 2.0 &&
              point[vAxis] <= high[vAxis] + 2.0)) {
            return false;
        }
    } else {
        if (any(lessThan(point, low - 2.0)) || any(greaterThan(point, high + 2.0))) {
            return false;
        }
        distance = abs(dot(shadowVector(face), point) - shadowValue(face + 3u));
        if (distance > 2.0) {
            return false;
        }
    }
    if (shadowStatic[face + 14u] != 0u) {
        return true;
    }
    int orientation = 0;
    uint edge = shadowStatic[face + 15u];
    uint count = shadowStatic[face + 16u];
    for (uint i = 0u; i < count; i++, edge += 5u) {
        float cross = shadowValue(edge + 2u) * (point[vAxis] - shadowValue(edge + 1u)) -
                      shadowValue(edge + 3u) * (point[uAxis] - shadowValue(edge));
        float margin = shadowValue(edge + 4u);
        if (cross > margin) {
            if (orientation < 0) {
                return false;
            }
            orientation = 1;
        } else if (cross < -margin) {
            if (orientation > 0) {
                return false;
            }
            orientation = -1;
        }
    }
    return true;
}
/* Each GPU cell selects the closest receiver independently. The CPU's
 * per-band coherence hint may reuse a slightly farther face within its
 * two-unit tolerance at a crease; that optimization is intentionally not
 * carried between independent GPU invocations. */
uint shadowReceiver(vec3 point)
{
    if (shadowStatic[3] == 0u) {
        return SHADOW_NONE;
    }
    /* The CPU BSP builder stops at depth24. Depth-first traversal therefore
     * needs at most25 pending nodes, even where receiver tolerance visits
     * both children. Avoid recursion and keep a small bounded private stack. */
    uint stack[32];
    uint count = 1u;
    stack[0] = 0u;
    uint best = SHADOW_NONE;
    float bestDistance = 3.0;
    while (count != 0u) {
        uint node = shadowStatic[2] + stack[--count] * 12u;
        if (any(lessThan(point, shadowVector(node) - 2.0)) ||
            any(greaterThan(point, shadowVector(node + 3u) + 2.0))) {
            continue;
        }
        uint first = shadowStatic[node + 7u];
        uint end = first + shadowStatic[node + 8u];
        for (uint i = first; i < end; i++) {
            uint face = shadowStatic[shadowStatic[4] + i];
            float distance;
            if (shadowMatches(face, point, distance) && distance < bestDistance) {
                bestDistance = distance;
                best = face;
            }
        }
        uint left = shadowStatic[node + 9u];
        uint right = shadowStatic[node + 10u];
        uint axis = shadowStatic[node + 11u];
        float split = shadowValue(node + 6u);
        if (left != SHADOW_NONE && point[axis] <= split + 2.0) {
            stack[count++] = left;
        }
        if (right != SHADOW_NONE && point[axis] >= split - 2.0) {
            stack[count++] = right;
        }
    }
    return best;
}
uint shadowGroundPage(vec2 point)
{
    uint capacity = shadowStatic[8];
    if (capacity == 0u) {
        return SHADOW_NONE;
    }
    ivec2 page = ivec2(floor(point / 256.0));
    uint slot = (uint(page.x) * 0x9e3779b1u ^ uint(page.y) * 0x85ebca77u) & (capacity - 1u);
    uint table = shadowStatic[7];
    for (uint probe = 0u; probe < capacity; probe++) {
        uint entry = shadowStatic[table + slot];
        if (entry == 0u) {
            return SHADOW_NONE;
        }
        uint record = shadowStatic[5] + (entry - 1u) * 4u;
        if (int(shadowStatic[record]) == page.x && int(shadowStatic[record + 1u]) == page.y) {
            return shadowStatic[record + 2u];
        }
        slot = (slot + 1u) & (capacity - 1u);
    }
    return SHADOW_NONE;
}
uint shadowDynamicIndex(ivec2 point, uint rectangle, uint stride)
{
    ivec2 origin = ivec2(shadowFrame[rectangle], shadowFrame[rectangle + 1u]);
    ivec2 size = ivec2(shadowFrame[rectangle + 2u], shadowFrame[rectangle + 3u]);
    point -= origin;
    if (any(lessThan(point, ivec2(0))) || any(greaterThanEqual(point, size))) {
        return SHADOW_NONE;
    }
    return shadowFrame[rectangle + 4u] + uint(point.y * size.x + point.x) * stride;
}
float shadowDynamicLight(vec2 point, float height, vec2 gradient)
{
    ivec2 origin = ivec2(floor(point - 0.5));
    vec2 fraction = point - 0.5 - vec2(origin);
    float coverage = 0.0;
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            ivec2 cell = origin + ivec2(x, y);
            uint index = shadowDynamicIndex(cell, 17u, 1u);
            if (index == SHADOW_NONE) {
                continue;
            }
            float receiver = height + (float(cell.x) + 0.5 - point.x) * gradient.x +
                            (float(cell.y) + 0.5 - point.y) * gradient.y;
            float weight = (x != 0 ? fraction.x : 1.0 - fraction.x) *
                           (y != 0 ? fraction.y : 1.0 - fraction.y);
            if (shadowFrameValue(index) > receiver + 2.5) {
                coverage += weight;
            }
        }
    }
    return coverage;
}
float shadowDynamicContact(vec2 point, float height)
{
    const ivec2 offsets[8] = ivec2[8](ivec2(-3, 0), ivec2(3, 0), ivec2(0, -3), ivec2(0, 3),
                                    ivec2(-2, -2), ivec2(2, -2), ivec2(-2, 2), ivec2(2, 2));
    ivec2 cell = ivec2(floor(point));
    float coverage = 0.0;
    for (int i = 0; i < 8; i++) {
        uint index = shadowDynamicIndex(cell + offsets[i], 22u, 2u);
        if (index == SHADOW_NONE || shadowFrameValue(index + 1u) <= height + 2.5) {
            continue;
        }
        float gap = shadowFrameValue(index) - height;
        if (gap < -2.5) {
            continue;
        }
        gap = max(gap, 0.0);
        if (gap < 96.0) {
            coverage += 1.0 - gap / 96.0;
        }
    }
    return coverage * 0.125;
}
uint shadowDynamic(vec3 point, vec3 normal)
{
    vec2 light = point.xz + vec2(0.45, 0.30) * point.y;
    vec2 map = vec2(shadowFrameValue(5u), shadowFrameValue(6u));
    vec2 delta = abs(light - map - 2048.0);
    float distance = max(delta.x, delta.y);
    if (distance >= 2048.0) {
        return 0u;
    }
    float denominator = normal.y - 0.45 * normal.x - 0.30 * normal.z;
    float magnitude = dot(abs(normal), vec3(1.0));
    float shade = 0.0;
    if (abs(denominator) > magnitude * 0.0001) {
        vec2 gradient = -normal.xz * 8.0 / denominator;
        shade = shadowDynamicLight((light - map) * 0.125, point.y, gradient) * 78.0;
    }
    float contact = shadowDynamicContact((point.xz - map) * 0.125, point.y) * 34.0;
    contact *= magnitude > 0.0 ? abs(normal.y) / magnitude : 0.0;
    shade += contact * (1.0 - shade / 255.0);
    if (distance > 1536.0) {
        shade *= (2048.0 - distance) / 512.0;
    }
    return uint(shade + 0.5);
}
uint sampleCachedView(vec3 relative, float footprint)
{
    if (shadowFrame[0] == 0u) {
        return 0u;
    }
    vec2 delta = abs(relative.xz + vec2(0.45, 0.30) * relative.y);
    float distance = max(delta.x, delta.y);
    if (distance >= 2048.0) {
        return 0u;
    }
    float fade = distance <= 1536.0 ? 1.0 : (2048.0 - distance) / 512.0;
    vec3 point = relative + shadowFrameVector(2u);
    vec3 normal = vec3(0.0, 1.0, 0.0);
    uint shade = 0u;
    bool valid = false;
    if (abs(point.y) <= 2.0) {
        valid = true;
        uint texture = shadowGroundPage(point.xz);
        if (texture != SHADOW_NONE) {
            shade = shadowTexture(texture, point.xz, footprint);
        }
    } else {
        uint receiver = shadowReceiver(point);
        if (receiver != SHADOW_NONE) {
            valid = true;
            uint face = shadowStatic[0] + receiver * 20u;
            vec2 uv = vec2(point[shadowStatic[face + 10u]], point[shadowStatic[face + 11u]]);
            shade = shadowTexture(shadowStatic[face + 17u], uv, footprint);
            normal = shadowVector(face);
        }
    }
    shade = uint(float(shade) * fade + 0.5);
    if (shadowFrame[1] != 0u && valid) {
        vec2 light = point.xz + vec2(0.45, 0.30) * point.y;
        vec2 low = vec2(shadowFrameValue(7u), shadowFrameValue(8u));
        vec2 high = vec2(shadowFrameValue(9u), shadowFrameValue(10u));
        bool casting = all(greaterThanEqual(light, low - 8.0)) && all(lessThanEqual(light, high + 8.0));
        vec3 contactLow = shadowFrameVector(11u);
        vec3 contactHigh = shadowFrameVector(14u);
        bool contact = all(greaterThanEqual(point.xz, contactLow.xz - 32.0)) &&
                       all(lessThanEqual(point.xz, contactHigh.xz + 32.0)) &&
                       point.y + 96.0 > contactLow.y;
        if ((casting || contact) && point.y + 2.5 < contactHigh.y) {
            shade = max(shade, shadowDynamic(point, normal));
        }
    }
    return shade;
}
