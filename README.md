# ⚔️ War in Pandora 

O mundo de **Pandora** enfrenta tempos de conflito entre os povos **Ya’nki** e **Chi’ni**. Em uma missão secreta, os **Chi’ni** enviaram **balões espiões** (`v_O_v`) para monitorar o território inimigo. No entanto, os **Ya’nki** identificaram os objetos voadores e enviaram **caças de combate** (`|-T-|`), equipados com bombas de detonação por afastamento, para derrubá-los.  

Os **balões espiões** registram imagens do território antes de serem abatidos e enviam as fotos para a **base de observação** dos Chi’ni. Essas imagens podem revelar **instalações militares estratégicas** ou **áreas irrelevantes**. Para avaliar a importância das capturas, cada foto recebe um índice numérico: **quanto maior o valor, mais relevante é a informação obtida**.  

## 📷 **Capacidade de Observação dos Balões**  

Cada balão possui um **sensor fotográfico** com um alcance definido por três parâmetros:  

- **Raio de abrangência** (**r**): determina a largura da região capturada.  
- **Profundidade inicial** (**p**): define a distância abaixo do balão onde a captura começa a ficar nítida.  
- **Faixa de foco** (**h**): representa a região onde a imagem continua clara e detalhada.  

🔴 **A câmera dos balões registra apenas os elementos dentro da área pontilhada em vermelho**, garantindo que informações cruciais sejam transmitidas à base Chi’ni.  

## 🏹 **Os Avatares**  

Os **Avatares** são representados por **bonecos uniformizados** e desempenham um papel fundamental no desenrolar do conflito. Sua presença pode ser decisiva em momentos estratégicos da guerra.  

---

### **Instruções de Execução**  

1. **Clonar o repositório na sua máquina:**  
   ```bash
   git clone git@github.com:fonsecalarissamaria/War_in_Pandora.git
   ```  

2. **Executar o script para extrair os arquivos e compilar:**  
   ```bash
   python3 et.py -Z -c -t
   ```  

   - O ZIP com os arquivos-fonte será extraído na pasta `alunos/larissamsf/`.  
   - O Makefile será executado automaticamente.  
   - Os arquivos de saída serão gerados dentro de `alunos/larissamsf/o/`.  

3. **Verificar as saídas dos testes:**  
   - Acesse a pasta de saída:  
     ```bash
     cd alunos/larissamsf/o/
     ```  
   - Os arquivos gerados (SVG, TXT, relatório de saída) estarão disponíveis para análise.
