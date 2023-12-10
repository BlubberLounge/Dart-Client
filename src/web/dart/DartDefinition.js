/**
 *
 *
 *
 */
class DartDefinition
{
    static maxSets = 1;
    static maxLegsPerSet = 1;
    static maxTurnsPerLeg = 100;
    static maxThrowsPerTurn = 3;

    static maxPlayers = 4;

    static apiBasePath = '/api/v1';

    static dartboardRadius = 200;   // centimeter
    static dartboardDiameter = this.dartboardRadius * 2;

    static fieldOrder1 = [1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, 20];
    static fieldOrder20 = [20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5];

    static fieldMultiplier = [1, 1, 1, 3, 1, 2, 0];

    static fieldSizeDeg = 360 / this.fieldOrder1.length;
    static fieldSizeRad = (Math.PI * 2) / this.fieldOrder1.length;

    static ringRadiiRelative = { // relative and normalized
        start: 0,            // start - used for easier index mapping in calculations
        doubleBullseye: .06, //  6% = Double Bullseye
        bullseye: .07,       //  7% = Bullseye
        innerSingle: .39,    // 39% = Inner Single
        tripple: .09,        //  9% = Tripple
        outerSingle: .30,    // 30% = Outer Single
        double: .09,         //  9% = Double
        out: Infinity,       // in% = Out
    };

    static ringRadiiAbsolute = { // absolute and denormalized
        doubleBullseye: this._getAbsoluteRadii('doubleBullseye'),
        bullseye: this._getAbsoluteRadii('bullseye'),
        innerSingle: this._getAbsoluteRadii('innerSingle'),
        tripple: this._getAbsoluteRadii('tripple'),
        outerSingle: this._getAbsoluteRadii('outerSingle'),
        double: this._getAbsoluteRadii('double'),
        out: this._getAbsoluteRadii('out'),
    };


    constructor()
    {
        // create singleton at a later point for performance optimization
        // if(DartDefinition.instance)
        //     return DartDefinition.instance;

        // DartDefinition.instance = this;
    }

    static getRingRadiiRelativeArray()
    {
        return Object.entries(this.ringRadiiRelative).map( r => r[1] );
    }

    static getRingRadiiAbsoluteArray()
    {
        return Object.entries(this.ringRadiiAbsolute).map( r => r[1] );
    }

    static _getAbsoluteRadii(ringRadiusRelative)
    {
        let relativeRadii = Object.entries(this.ringRadiiRelative);
        let index = relativeRadii.findIndex( r => r[0] == ringRadiusRelative);
        let size = relativeRadii[index][1];

        let start = 0;
        for(let i = 1; i < index; i++)
            start += relativeRadii[i][1];

        return {
            size: size * this.dartboardRadius,
            start: start * this.dartboardRadius,
            end: (start + size) * this.dartboardRadius,
        };
    }
}
