export class Piece {
    m_style : string;
    m_style_degra : string;
    m_id : string;
    
    constructor(style : string, style_degra : string, id : string) {
        this.m_style = style;
        this.m_style_degra = style_degra;
        this.m_id = id;
    }
    
    getStyle() {
        return this.m_style;
    }
    
    getStyleDegra() {
        return this.m_style_degra;
    }
    
    getId() {
        return this.m_id;
    }
}