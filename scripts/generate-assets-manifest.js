#!/usr/bin/env node
/**
 * Generates assets.json manifest for game assets folders.
 * Usage: node scripts/generate-assets-manifest.js [game-path]
 * 
 * If no game-path is provided, it will scan all game directories in static/games/
 */

import { readdirSync, statSync, writeFileSync, existsSync } from 'fs';
import { join, relative } from 'path';

const STATIC_GAMES_DIR = 'static/games';

function getAllFiles(dir, baseDir = dir) {
  const files = [];
  
  if (!existsSync(dir)) {
    return files;
  }

  const entries = readdirSync(dir, { withFileTypes: true });
  
  for (const entry of entries) {
    const fullPath = join(dir, entry.name);
    
    if (entry.isDirectory()) {
      files.push(...getAllFiles(fullPath, baseDir));
    } else {
      // Get path relative to the assets folder
      const relativePath = relative(baseDir, fullPath);
      files.push(relativePath);
    }
  }
  
  return files;
}

function generateManifest(gamePath) {
  const assetsDir = join(gamePath, 'assets');
  const manifestPath = join(gamePath, 'assets.json');
  
  const files = getAllFiles(assetsDir);
  
  const manifest = {
    files: files.sort()
  };
  
  writeFileSync(manifestPath, JSON.stringify(manifest, null, 2) + '\n');
  console.log(`Generated ${manifestPath} with ${files.length} file(s)`);
}

function main() {
  const args = process.argv.slice(2);
  
  if (args.length > 0) {
    // Generate for specific game path
    for (const gamePath of args) {
      if (existsSync(gamePath)) {
        generateManifest(gamePath);
      } else {
        console.error(`Path not found: ${gamePath}`);
      }
    }
  } else {
    // Generate for all games in static/games/
    if (!existsSync(STATIC_GAMES_DIR)) {
      console.log(`No ${STATIC_GAMES_DIR} directory found`);
      return;
    }
    
    const entries = readdirSync(STATIC_GAMES_DIR, { withFileTypes: true });
    
    for (const entry of entries) {
      if (entry.isDirectory()) {
        const gamePath = join(STATIC_GAMES_DIR, entry.name);
        generateManifest(gamePath);
      }
    }
  }
}

main();
