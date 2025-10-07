cool = function (data) {
    console.log(data);

    let name = document.getElementById("name");
    name.innerHTML = data.name;
    
    if (data.glasses && Array.isArray(data.glasses)) {
        renderComparisons(data.glasses);
    }
};

// ===== Type 2 generator (vergelijkingen tussen glazen) =====
function renderComparisons(glasses) {
    const host = document.getElementById("comparisons");
    host.innerHTML = "";

    // Groepeer glazen per tag
    const byTag = new Map();
    glasses.forEach(g => {
        const tags = g.tags || [];
        tags.forEach(t => {
            const tag = String(t || "").trim().toLowerCase();
            if (!tag) return;
            if (!byTag.has(tag)) byTag.set(tag, []);
            byTag.get(tag).push(g);
        });
    });

    let idx = 0;
    for (const [tag, arr] of byTag.entries()) {
        if (arr.length < 2) continue;

        const set = document.createElement("div");
        set.className = "cmp-set";

        // Tag titel (groot en gecentreerd)
        const title = document.createElement("div");
        title.className = "set-title";
        title.textContent = tag;
        set.appendChild(title);

        // Maak vergelijkingen tussen alle paren
        for (let i = 0; i < arr.length; i++) {
            for (let j = i + 1; j < arr.length; j++) {
                const A = arr[i], B = arr[j];
                const aName = A.naam || A.name || A.id || `G${i + 1}`;
                const bName = B.naam || B.name || B.id || `G${j + 1}`;
                const base = `cmp_${idx}`;

                // Verborgen velden voor context (met _ prefix voor custom data)
                set.appendChild(createHidden(`_cmp_${idx}_tag`, tag));
                set.appendChild(createHidden(`_cmp_${idx}_glass_a`, "glass " + A.id || aName));
                set.appendChild(createHidden(`_cmp_${idx}_glass_b`, "glass " + B.id || bName));

                // Vergelijkingsrij
                const row = document.createElement("div");
                row.className = "cmp-row";

                const left = document.createElement("div");
                left.className = "cmp-glass";
                left.textContent = "glas " + aName;

                const ops = document.createElement("div");
                ops.className = "cmp-ops";
                ops.appendChild(createRadio(`_cmp_${idx}_rel`, `${base}_gt`, "A_gt", ">", true));
                ops.appendChild(createRadio(`_cmp_${idx}_rel`, `${base}_eq`, "eq", "=", false));
                ops.appendChild(createRadio(`_cmp_${idx}_rel`, `${base}_lt`, "B_gt", "<", false));

                const right = document.createElement("div");
                right.className = "cmp-glass";
                right.textContent = "glas " + bName;

                row.appendChild(left);
                row.appendChild(ops);
                row.appendChild(right);
                set.appendChild(row);

                // Zekerheid knoppen
                const extra = document.createElement("div");
                extra.className = "cmp-extra";
                extra.insertAdjacentHTML('beforeend', '<span>Zekerheid:</span>');
                extra.appendChild(createCertRadio(`_cmp_${idx}_zekerheid`, `${base}_c1`, "1", "1"));
                extra.appendChild(createCertRadio(`_cmp_${idx}_zekerheid`, `${base}_c2`, "2", "2"));
                extra.appendChild(createCertRadio(`_cmp_${idx}_zekerheid`, `${base}_c3`, "3", "3"));
                set.appendChild(extra);

                host.appendChild(set);
                idx++;
            }
        }
    }
}

// Helper functies voor het maken van form elementen
function createHidden(name, value) {
    const h = document.createElement("input");
    h.type = "hidden";
    h.name = name;
    h.value = value;
    return h;
}

function createRadio(name, id, value, symbol, required) {
    const wrap = document.createElement("div");
    const r = document.createElement("input");
    r.type = "radio";
    r.name = name;
    r.id = id;
    r.value = value;
    if (required) r.required = true;
    const l = document.createElement("label");
    l.setAttribute("for", id);
    l.textContent = symbol;
    wrap.appendChild(r);
    wrap.appendChild(l);
    return wrap;
}

function createCertRadio(name, id, value, text) {
    const wrap = document.createElement("div");
    wrap.className = "cmp-cert";
    const r = document.createElement("input");
    r.type = "radio";
    r.name = name;
    r.id = id;
    r.value = value;
    r.required = true;
    const l = document.createElement("label");
    l.setAttribute("for", id);
    l.textContent = text;
    wrap.appendChild(r);
    wrap.appendChild(l);
    return wrap;
}

window.UserData.onReady(cool);