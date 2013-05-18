//
//  Shader.fsh
//  space-shooter
//
//  Created by デービッドシアウ on 2013/05/19.
//
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
