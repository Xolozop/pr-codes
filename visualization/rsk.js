async function classicalRSK(perm) {
    let P = [];
    let Q = [];

    P.push([perm[0]]);
    Q.push([1]); // ставим первый элемент в начало таблицы

    await visualize(P, Q); // Визуализируем начальное состояние
    await delay(1000); // Задержка в 1000 мс (1 секунда)

    for (let i = 1; i < perm.length; i++) {
        let k = 0;
        let x = P[k].length - 1;
        let a = perm[i];
        let pushedElement = a; // Сохраняем вставляемый элемент
        let n;

        while (k < P.length) {
            n = 0;
            if (a > P[k][P[k].length - 1]) {
                P[k].push(a);
                Q[k].push(i + 1); // если A - наибольший в строке элемент, добавляем A в конец
                break;
            } else {
                if (x < 1 || x > P[k].length - 1) {
                    x = P[k].length - 1;
                }
                while (x > 0) {
                    if (a < P[k][x] && a > P[k][x - 1]) {
                        break;
                    }
                    x--;
                }
                n = P[k][x];
                P[k][x] = a;
                a = n;
                k++;
            }
        }
        if (k >= P.length) {
            P.push([a]);
            Q.push([i + 1]);
        }

        await visualize(P, Q, pushedElement, n); // Визуализируем после каждого шага
        await delay(1000); // Задержка в 1000 мс (1 секунда)
    }
    return [P, Q]; // возвращаем массив из двух таблиц
}

function visualize(P, Q, pushedElement, poppedElement) {
    return new Promise((resolve) => {
        const visualizationDiv = document.getElementById('visualization');
        visualizationDiv.innerHTML = ''; // очищаем предыдущую визуализацию

        // Визуализация таблицы P
        const tableauP = document.createElement('div');
        tableauP.className = 'tableau';
        tableauP.innerHTML = '<h3>Tableau P</h3>';
        P.forEach(row => {
            const rowDiv = document.createElement('div');
            row.forEach(cell => {
                const cellDiv = document.createElement('div');
                cellDiv.className = 'cell';
                cellDiv.textContent = cell;

                // Подсветка вставляемого элемента
                if (cell === pushedElement) {
                    cellDiv.style.backgroundColor = 'lightgreen';
                }
                // Подсветка выталкиваемого элемента
                if (cell === poppedElement) {
                    cellDiv.style.backgroundColor = 'lightcoral';
                }

                rowDiv.appendChild(cellDiv);
            });
            tableauP.appendChild(rowDiv);
        });
        visualizationDiv.appendChild(tableauP);

        // Визуализация таблицы Q
        const tableauQ = document.createElement('div');
        tableauQ.className = 'tableau';
        tableauQ.innerHTML = '<h3>Tableau Q</h3>';
        Q.forEach(row => {
            const rowDiv = document.createElement('div');
            row.forEach(cell => {
                const cellDiv = document.createElement('div');
                cellDiv.className = 'cell';
                cellDiv.textContent = cell;

                // Подсветка выталкиваемого элемента
                if (cell === poppedElement) {
                    cellDiv.style.backgroundColor = 'lightcoral';
                }

                rowDiv.appendChild(cellDiv);
            });
            tableauQ.appendChild(rowDiv);
        });
        visualizationDiv.appendChild(tableauQ);

        resolve(); // Завершаем промис
    });
}

function delay(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

// Пример использования
const permutation = [3, 1, 2, 4];
classicalRSK(permutation);
