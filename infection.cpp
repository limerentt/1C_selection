#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <map>


using namespace std;
using ll = long long;


class graph {
private:
    map<ll, set<ll>> neis;  // Список соседей для каждой вершины
    set<ll> infected;   // Множество инфецированных


    /* Проверка заразится ли эта вершина от уже инфецированной */
    bool has_second_infected_nei(const set<ll>& cur_infected, ll v, ll infected_nei) {
        return ranges::any_of(neis[v],
                              [&](ll u) { return u != infected_nei && cur_infected.contains(u); });
    }


    /* Получаем множество всех вершин, которые заразятся от v */
    set<ll> get_infected(set<ll> cur_infected, ll v) {
        set<ll> to_run_through;     // множество заразившихся от v вершин
        cur_infected.insert(v);
        to_run_through.insert(v);

        while (!to_run_through.empty()) {   // заражаем всех кого можем
            ll cur_v = *(to_run_through.begin());
            to_run_through.erase(cur_v);
            cur_infected.insert(cur_v);

            for (auto cur_v_son: neis[cur_v]) {
                if (!cur_infected.contains(cur_v_son) &&
                    has_second_infected_nei(cur_infected,cur_v_son,cur_v)) {
                    cur_infected.insert(cur_v_son);
                    to_run_through.insert(cur_v_son);
                }
            }
        }

        return cur_infected;
    }


    /* Получаем множество вершин, которые сами начали разносить болезнь */
    set<ll> get_initiators() {
        for (const auto& p: neis) {
            if (p.second.size() == 1) {
                infected.insert(p.first);
            }
        }

        set<ll> initiators = infected;

        // заражаем все листы, иначе они не смогут ни от кого заразиться
        for (auto v: initiators) {
            infected = get_infected(infected, v);
        }

        // заражаем вершины пока не заразим весь граф
        while (infected.size() < neis.size()) {
            set<ll> new_infected = infected;
            ll v = (*(neis.begin())).first;
            for (const auto& p: neis) {
                set<ll> v_infected = get_infected(infected, p.first);
                if (v_infected.size() > new_infected.size()) {
                    new_infected = v_infected;
                    v = p.first;
                }
            }
            initiators.insert(v);
            infected = new_infected;
        }

        return initiators;
    }

public:
    void build_graph() {
        ll M;
        cin >> M;

        for (ll i = 0; i < M; ++i) {
            ll u, v;
            cin >> u >> v;
            neis[u].insert(v);
            neis[v].insert(u);
        }
    }

    void print_initiators() {
        set<ll> initiators = get_initiators();
        cout << initiators.size() << '\n';
        for (auto v: initiators) {
            cout << v << ' ';
        }
    }
};

int main() {
    graph G = graph();
    G.build_graph();
    G.print_initiators();
}
