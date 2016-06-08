
out float density;

uniform vec3 world_position;

void main(){
    density = -world_position.y;
}