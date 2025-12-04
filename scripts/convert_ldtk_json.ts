interface LdtkEntity {
    __identifier: string;
    __grid: [number, number];
    fieldInstances: { __identifier: string; __value: string }[];
}

interface LdtkLayer {
    __identifier: string;
    __type: string;
    __cWid: number;
    __cHei: number;
    intGridCsv: number[];
    entityInstances: LdtkEntity[];
}

interface LdtkLevel {
    pxWid: number;
    pxHei: number;
    layerInstances: LdtkLayer[];
}

interface AtomixLevel {
    width: number;
    height: number;
    nbrAtoms: number;
    map: boolean[][];
    atoms: { x: number; y: number; value: number }[];
    solution: { x: number; y: number; value: number }[];
}

async function convertLdtkToAtomix(inputPath: string, outputPath: string) {
    // Lire le fichier LDtk
    const data: LdtkLevel = await Bun.file(inputPath).json();

    // Trouver les layers
    const mapLayer = data.layerInstances.find((l) => l.__identifier === "Map")!;
    const atomLayer = data.layerInstances.find((l) => l.__identifier === "Atome")!;
    const soluceLayer = data.layerInstances.find((l) => l.__identifier === "Soluce")!;

    const width = mapLayer.__cWid;
    const height = mapLayer.__cHei;

    // Convertir la grille (1 = mur, 2 = vide)
    const map: boolean[][] = [];
    for (let y = 0; y < height; y++) {
        const row: boolean[] = [];
        for (let x = 0; x < width; x++) {
            const val = mapLayer.intGridCsv[y * width + x];
            row.push(val === 2); // 2 = traversable
        }
        map.push(row);
    }

    // Extraire les atomes
    const extractEntities = (layer: LdtkLayer) =>
        layer.entityInstances.map((e) => ({
            x: e.__grid[0],
            y: e.__grid[1],
            value: parseInt((e.fieldInstances.find((f) => f.__identifier === "Color")?.__value ?? "#FFFFFF").replace("#", ""), 16),
        }));

    const output: AtomixLevel = {
        width,
        height,
        nbrAtoms: extractEntities(soluceLayer).length,
        map,
        atoms: extractEntities(atomLayer),
        solution: extractEntities(soluceLayer),
    };

    // Écrire le fichier de sortie
    await Bun.write(outputPath, JSON.stringify(output, null, 2));

    console.log(`✅ Converti: ${inputPath} -> ${outputPath}`);
}

// Point d'entrée
const [inputPath, outputPath] = Bun.argv.slice(2);

if (!inputPath || !outputPath) {
    console.log("Usage: bun run convert_ldtk.ts <input.ldtkl> <output.json>");
    process.exit(1);
}

await convertLdtkToAtomix(inputPath, outputPath);